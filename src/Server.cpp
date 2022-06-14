#include "Server.hpp"

//~~ CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), _online(false), _startTime(std::time(NULL))
{
	struct protoent	*pe;
	struct pollfd	tmp;
	int				sockopt = 1;

	if ((pe = getprotobyname("tcp")) == NULL)
		stop(errno);
	_sock = socket(PF_INET, SOCK_STREAM, pe->p_proto); // settings TCP
	if (_sock == -1)
		stop(errno);
	tmp.fd = _sock; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	_fds.push_back(tmp);
  	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)
		stop(errno);
	_servSocket.sin_family = PF_INET; // address format IPV6
	_servSocket.sin_port = htons(_port); // convert port
	_servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (fcntl(_sock, F_SETFL, O_NONBLOCK) == -1) // server socket non blocking
		stop(errno);
	if (bind(_sock, reinterpret_cast<struct sockaddr*>(&_servSocket), sizeof(_servSocket)) == -1)
		stop(errno);
	if (listen(_sock, SOMAXCONN) == -1) // SOMAXCONN = max value
		stop(errno);
	initSupportedCommands();
}

void		Server::initSupportedCommands()
{
	_cmdList.push_back(Command("PASS",		2, &pass));
	_cmdList.push_back(Command("NICK",		2, &nick));
	_cmdList.push_back(Command("USER",		5, &user));
	_cmdList.push_back(Command("OPER",		3, &oper));
	_cmdList.push_back(Command("QUIT",		1, &quit));
	_cmdList.push_back(Command("JOIN",		2, &join));
	_cmdList.push_back(Command("PART",		2, &part));
	_cmdList.push_back(Command("MODE",		2, &mode));
	_cmdList.push_back(Command("TOPIC",		2, &topic));
	_cmdList.push_back(Command("NAMES",		1, &names));
	_cmdList.push_back(Command("LIST",		1, &list));
	_cmdList.push_back(Command("KICK",		3, &kick));
	_cmdList.push_back(Command("VERSION",	1, &version));
	_cmdList.push_back(Command("STATS",		1, &stats));
	_cmdList.push_back(Command("TIME",		1, &time));
	_cmdList.push_back(Command("ADMIN",		1, &admin));
	_cmdList.push_back(Command("INFO",		1, &info));
	_cmdList.push_back(Command("PRIVMSG",	3, &privmsg));
	_cmdList.push_back(Command("NOTICE",	3, &notice));
	_cmdList.push_back(Command("WHO",		1, &who));
	_cmdList.push_back(Command("WHOIS",		2, &whois));
	_cmdList.push_back(Command("WHOWAS",	2, &whowas));
	_cmdList.push_back(Command("KILL",		3, &kill));
	_cmdList.push_back(Command("PING",		2, &ping));
	_cmdList.push_back(Command("PONG",		2, &pong));
	_cmdList.push_back(Command("ERROR",		2, &error));
}

//~~ DESTRUCTOR

Server::~Server() {}

//~~ ACCESSOR

int			Server::getPort() const
{
	return _port;
}

std::string	Server::getPassword() const
{
	return _password;
}

std::string	Server::getStartTime() const
{
	return asctime(localtime(&_startTime));
}

Client*		Server::getClient(std::string name) const
{
	return _clientsByName.find(name)->second;
}

Channel*	Server::getChannel(std::string name) const
{
	return _channels.find(name)->second;
}

std::map<std::string, Client*> &	Server::getAllClients()
{
	return _clientsByName;
}

std::map<std::string, Channel*> &	Server::getAllChannels()
{
	return _channels;
}

//~~ METHODS

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock);
	
	newClient->setHost(inet_ntoa(_servSocket.sin_addr));
	_clientsBySock.insert(std::make_pair(sock, newClient));
}

void	Server::removeClient(Client *src, std::vector<struct pollfd>::iterator it)
{
	std::cout << "Someone is disconnecting : " << it->fd << std::endl; // test
	if (src->isRegistered() == true)
	{
		_oldClients.insert(std::make_pair(src->getNickname(), src));
		_clientsByName.erase(src->getNickname());
	}
	delete src;
	_clientsBySock.erase(it->fd);
	close(it->fd);
	_fds.erase(it);
}

void	Server::sendMessages()
{
	int		ret;
	Client	*client;

	for(std::vector<struct pollfd>::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if (client->hasOutput()) // request on this socket
		{
			ret = send(it->fd, client->getOutputBuffer(), strlen(client->getOutputBuffer()), 0);
			client->clearOutputBuffer();
			if (ret < 0 && errno == ECONNRESET) // deconnexion
				removeClient(client, it);
		}
	}
}

void		Server::executeCommand(std::vector<std::string>	cmdArgs, Client* sender)
{
	Command*	cmd = NULL;

	for(std::vector<Command>::iterator it = _cmdList.begin(); it != _cmdList.end(); it++)
	{
		if (!cmdArgs.front().compare(it->getName()))
			cmd = &(*it);
	}
	if (cmd == NULL)
		sender->addToOutputBuffer(ERR_UNKNOWNCOMMAND(sender->getNickname(), cmdArgs.front()));
	else if (sender->isRegistered() == false && cmd->getName().compare("USER") && cmd->getName().compare("PASS") && cmd->getName().compare("NICK"))
		sender->addToOutputBuffer(ERR_NOTREGISTERED(sender->getNickname()));
	else
	{
		cmd->fct(cmdArgs, sender, this);
		if (_clientsByName.find(sender->getNickname()) == _clientsByName.end() && sender->isRegistered() == true)
			_clientsByName.insert(std::make_pair(sender->getNickname(), sender));
	}
}

void		Server::executeRequest(Client* sender)
{
	for(size_t i = sender->getInputBuffer().find(CRLF); i != std::string::npos; i = sender->getInputBuffer().find(CRLF))
	{
		size_t						start = sender->getInputBuffer().find_first_not_of(" ");
		std::string					cmdLine = sender->getInputBuffer().substr(start, i - start);
		std::vector<std::string>	cmdArgs;

		if (sender->getInputBuffer().find(CRLF) >= MESSAGELEN - CRLF.size())
			cmdLine.erase(MESSAGELEN, cmdLine.size() - MESSAGELEN);
		if (!cmdLine.empty())
		{
			if (cmdLine[0] == ':')
				cmdLine.erase(0, cmdLine.find_first_not_of(' ', cmdLine.find_first_of(' ', 0)));
			for(size_t j = cmdLine.find_first_of(' '); j != std::string::npos; j = cmdLine.find_first_of(' '))
			{
				if (cmdLine[0] == ':')
					break;
				cmdArgs.push_back(cmdLine.substr(0, j));
				cmdLine.erase(0, cmdLine.find_first_not_of(' ', j));
			}
			if (!cmdLine.empty())
				cmdArgs.push_back(cmdLine);
			executeCommand(cmdArgs, sender);
		}
		sender->getInputBuffer().erase(0, i + CRLF.size());
	}
	if (sender->getInputBuffer().size() >= MESSAGELEN - CRLF.size())
	{
		sender->addToInputBuffer(CRLF.c_str());
		executeRequest(sender);
	}
}

void	Server::receiveMessages()
{
	int 	ret;
	char	buf[TCP_MAXWIN + 1];
	Client	*client;

	for(std::vector<struct pollfd>::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if ((it->revents | POLLHUP) == it->revents) // deconnexion
			removeClient(client, it);
		else if (it->revents == POLLIN)
		{
			ret = recv(it->fd, buf, TCP_MAXWIN, 0);
			if (ret > 0)
			{
				buf[ret] = '\0';
				client->addToInputBuffer(buf);
				executeRequest(client);
			}
		}
	}
}

void	Server::acceptConnexions()
{
	int 			ret;
	struct pollfd	tmp;

	do
	{
		socklen_t addrlen = sizeof(_servSocket);
		ret = accept(_fds[0].fd, reinterpret_cast<struct sockaddr*>(&_servSocket), &addrlen);
		if (ret > 0)
		{
			std::cout << "Someone is connecting: " << ret << std::endl; // test
			tmp.fd = ret;
			tmp.events = POLLIN;
			fcntl(tmp.fd, F_SETFL, O_NONBLOCK); // client socket non blocking
			_fds.push_back(tmp);
			addClient(ret);
		}
	} while (ret > 0);
}

void	Server::run()
{
	int	numberSockets; 
	
	_online = true;
	while (_online)
	{
		numberSockets = poll(&_fds[0], _fds.size(), 0); // return the number of socket with request and fill pollfd
		if (numberSockets == -1)
			stop(EXIT_FAILURE);
		acceptConnexions(); // the server accept connexions
		receiveMessages(); // the server retrieves the requests
		sendMessages(); // the server responds to requests
	}
	stop(EXIT_SUCCESS);
}

void	closeFd(struct pollfd rhs)
{
	close(rhs.fd);
}

void	Server::stop(int status)
{
	//	Je verrais plus un truc comme
	//	if (status)
	//	{
	//		envoyer des messages comme quoi le serv crash aux clients
	//		std::cerr << SERVER_NAME << "server exited with code: " << status << std::endl;
	//	}
	//	else
	//	{
	//		envoyer des messages comme quoi le serv ferme aux clients
	//		std::cout << SERVER_NAME << "server closed" << std::endl;
	//	}
	std::cout << "Clean exit with status : " << status << std::endl;
	perror(SERV_NAME);

	std::for_each(_fds.begin(), _fds.end(), closeFd);
	_fds.clear();
	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	for(std::map<int, Client*>::iterator it = _clientsBySock.begin(); it != _clientsBySock.end(); ++it)
		delete it->second;
	_clientsBySock.clear();
	for(std::map<std::string, Client*>::iterator it = _oldClients.begin(); it != _oldClients.end(); ++it)
		delete it->second;
   	_oldClients.clear();
	exit(status);
}

std::string	Server::currentTime()
{
	struct tm	*timeinfo;
	time_t		time;
	
	timeinfo = localtime(&time);
	return asctime(timeinfo);
}
