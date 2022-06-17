#include "Server.hpp"

//~~ CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), _online(false), _startTime(std::time(NULL))
{
	struct protoent	*pe;
	struct pollfd	tmp;
	int				sockopt = 1;

	if ((pe = getprotobyname("tcp")) == NULL)
		stop(errno);
	_fd = socket(PF_INET, SOCK_STREAM, pe->p_proto); // settings TCP
	if (_fd == -1)
		stop(errno);
	tmp.fd = _fd; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	_fdList.push_back(tmp);
  	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)
		stop(errno);
	_servSocket.sin_family = PF_INET; // address format IPV6
	_servSocket.sin_port = htons(_port); // convert port
	_servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1) // server socket non blocking
		stop(errno);
	if (bind(_fd, reinterpret_cast<struct sockaddr*>(&_servSocket), sizeof(_servSocket)) == -1)
		stop(errno);
	if (listen(_fd, SOMAXCONN) == -1) // SOMAXCONN = max value
		stop(errno);

	initSupportedCommands();
}

void		Server::createLogFile()
{
	
}

void		Server::initSupportedCommands()
{
	_commands.insert(std::make_pair("PASS", &irc_pass));
	_commands.insert(std::make_pair("NICK", &irc_nick));
	_commands.insert(std::make_pair("USER", &irc_user));
	_commands.insert(std::make_pair("OPER", &irc_oper));
	_commands.insert(std::make_pair("QUIT", &irc_quit));
	_commands.insert(std::make_pair("JOIN", &irc_join));
	_commands.insert(std::make_pair("PART", &irc_part));
	_commands.insert(std::make_pair("MODE", &irc_mode));
	_commands.insert(std::make_pair("TOPIC", &irc_topic));
	_commands.insert(std::make_pair("NAMES", &irc_names));
	_commands.insert(std::make_pair("LIST", &irc_list));
	_commands.insert(std::make_pair("KICK", &irc_kick));
	_commands.insert(std::make_pair("VERSION", &irc_version));
	_commands.insert(std::make_pair("STATS", &irc_stats));
	_commands.insert(std::make_pair("TIME", &irc_time));
	_commands.insert(std::make_pair("ADMIN", &irc_admin));
	_commands.insert(std::make_pair("INFO", &irc_info));
	_commands.insert(std::make_pair("LUSERS", &irc_lusers));
	_commands.insert(std::make_pair("MOTD", &irc_motd));
	_commands.insert(std::make_pair("PRIVMSG", &irc_privmsg));
	_commands.insert(std::make_pair("NOTICE", &irc_notice));
	_commands.insert(std::make_pair("WHO", &irc_who));
	_commands.insert(std::make_pair("WHOIS", &irc_whois));
	_commands.insert(std::make_pair("WHOWAS", &irc_whowas));
	_commands.insert(std::make_pair("KILL", &irc_kill));
	_commands.insert(std::make_pair("PONG", &irc_pong));
}

//~~ DESTRUCTOR

Server::~Server() {}

//~~ SERVER

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

std::string	Server::getCurrentTime() const
{
	time_t	currentTime = time(NULL);
	
	return asctime(localtime(&currentTime));
}

//~~ CLIENT

Client*		Server::getClient(std::string name) const
{
	return _clientsByName.find(name)->second;
}

std::map<std::string, Client*> &	Server::getAllClients()
{
	return _clientsByName;
}

int	Server::getOpsNumber()
{
	int	num = 0;

	for(std::map<std::string, Client*>::iterator it = _clientsByName.begin(); it != _clientsByName.end(); ++it)
		if (it->second->isServOperator())
			num++;
	return (num);
}

int	Server::getNonRegisteredNumber()
{
	int	num = 0;

	for(std::map<std::string, Client*>::iterator it = _clientsByName.begin(); it != _clientsByName.end(); ++it)
		if (!(it->second->isRegistered()))
			num++;
	return (num);
}

std::map<std::string, Client*> &	Server::getOldNickname()
{
	return _oldNickname;
}

//~~ CHANNEL

Channel*	Server::getChannel(std::string name) const
{
	if (_channels.find(name) == _channels.end())
		return (NULL);
	else
		return (_channels.find(name)->second);
}

std::map<std::string, Channel*> &	Server::getAllChannels()
{
	return _channels;
}

Channel*	Server::newChannel(std::string name, Client* founder)
{
	Channel* channel = new Channel(name);
	_channels.insert(std::make_pair(name, channel));
	channel->addOperator(founder);
	return channel;
}

//~~ SERVER MAIN

void	Server::run()
{
	int	numberSockets; 
	
	_logFile.open( (std::string(SERV_NAME) + ".log").c_str() , std::ios_base::out | std::ios_base::trunc );
	if ( _logFile.is_open() == false )
		stop(errno);

	_online = true;
	while (_online)
	{
		numberSockets = poll(&_fdList[0], _fdList.size(), 0); // return the number of socket with request and fill pollfd
		if (numberSockets == -1)
			stop(EXIT_FAILURE);
		acceptConnexions(); // the server accept connexions
		receiveMessages(); // the server retrieves the requests
		sendMessages(); // the server responds to requests
	}
	stop(EXIT_SUCCESS);
}

void	Server::acceptConnexions()
{
	int 			ret;
	struct pollfd	tmp;

	do
	{
		socklen_t addrlen = sizeof(_servSocket);
		ret = accept(_fdList[0].fd, reinterpret_cast<struct sockaddr*>(&_servSocket), &addrlen);
		if (ret > 0)
		{
			std::cout << "Someone is connecting: " << ret << std::endl; // test
			tmp.fd = ret;
			tmp.events = POLLIN;
			fcntl(tmp.fd, F_SETFL, O_NONBLOCK); // client socket non blocking
			_fdList.push_back(tmp);
			addClient(ret);
		}
	} while (ret > 0);
}

void	Server::receiveMessages()
{
	int 	ret;
	char	buf[TCP_MAXWIN + 1];
	Client	*client;

	for(std::vector<struct pollfd>::iterator it = _fdList.begin() + 1; it < _fdList.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if ((it->revents | POLLHUP) == it->revents) // deconnexion
			removeClient(client);
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

void	Server::sendMessages()
{
	int		ret;
	Client	*client;

	for(std::vector<struct pollfd>::iterator it = _fdList.begin() + 1; it < _fdList.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if (client->hasOutput()) // request on this socket
		{
			ret = send(it->fd, client->getOutputBuffer(), strlen(client->getOutputBuffer()), 0);
			client->clearOutputBuffer();
			if (ret < 0 && errno == ECONNRESET) // deconnexion
				removeClient(client);
		}
	}
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

	std::for_each(_fdList.begin(), _fdList.end(), closeFd);
	_fdList.clear();
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

//~~ SERVER UTILS

void		Server::executeCommand(std::vector<std::string>	cmdArgs, Client* sender)
{
	std::map<std::string, command_t>::iterator it = _commands.find(cmdArgs.front());
	if (it == _commands.end())
		sender->addToOutputBuffer(ERR_UNKNOWNCOMMAND(sender->getNickname(), cmdArgs.front()));
	else if (sender->isRegistered() == false && it->first.compare("USER") && it->first.compare("PASS") && it->first.compare("NICK"))
		sender->addToOutputBuffer(ERR_NOTREGISTERED(sender->getNickname()));
	else
	{
		command_t fct = it->second;
		fct(cmdArgs, sender, this);
		if (_clientsByName.find(sender->getNickname()) == _clientsByName.end() && sender->isRegistered() == true)
		{
			_clientsByName.insert(std::make_pair(sender->getNickname(), sender));
			sendWelcome(sender);
		}
		else if (_clientsByName.find(sender->getNickname()) != _clientsByName.end() && _clientsByName.find(sender->getNickname())->second != sender)
		{
			//ERROR call
			removeClient(sender);
		}
	}
}

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock);
	
	newClient->setHost(inet_ntoa(_servSocket.sin_addr));
	_clientsBySock.insert(std::make_pair(sock, newClient));
}

void	Server::removeClient(Client *src)
{
	std::vector<struct pollfd>::iterator it = _fdList.begin();
	while (it->fd != src->getSock())
		it++;
	std::cout << "Someone is disconnecting : " << it->fd << std::endl; // test
	if (src->isRegistered() == true && _clientsByName.find(src->getNickname())->second == src)
	{
		_oldClients.insert(std::make_pair(src->getNickname(), src));
		_clientsByName.erase(src->getNickname());
	}
	_clientsBySock.erase(it->fd);
	close(it->fd);
	_fdList.erase(it);
	delete src;
}

void	Server::sendWelcome(Client* sender)
{
	std::string name = sender->getNickname();

	sender->addToOutputBuffer(RPL_WELCOME(name));
	sender->addToOutputBuffer(RPL_YOURHOST(name));
	sender->addToOutputBuffer(RPL_CREATED(name));
	sender->addToOutputBuffer(RPL_MYINFO(name));
	sender->addToOutputBuffer(RPL_ISUPPORT(name));
	sender->addToOutputBuffer(RPL_LUSERCLIENT(name, this));
	sender->addToOutputBuffer(RPL_LUSEROP(name, this));
	sender->addToOutputBuffer(RPL_LUSERUNKNOWN(name, this));
	sender->addToOutputBuffer(RPL_LUSERCHANNELS(name, this));
	sender->addToOutputBuffer(RPL_LUSERME(name, this));
	sender->addToOutputBuffer(RPL_MOTDSTART(name));
	sender->addToOutputBuffer(RPL_MOTD(name));
	sender->addToOutputBuffer(RPL_ENDOFMOTD(name));
	sender->addToOutputBuffer(RPL_UMODEIS(name, sender));
}

void		addLog(std::string message, int type)
{
	// switch case
}
