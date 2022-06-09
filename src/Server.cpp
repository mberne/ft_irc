#include "Server.hpp"

//~~ CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), online(false)
{
	// stats ?
	struct protoent	*pe;
	struct pollfd	tmp;
	int				sockopt = 1;

	if ((pe = getprotobyname("tcp")) == NULL)
		stop(errno);
	sock = socket(PF_INET, SOCK_STREAM, pe->p_proto); // settings TCP
	if (sock == -1)
		stop(errno);
	tmp.fd = sock; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	fds.push_back(tmp);
  	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)
		stop(errno);
	servSocket.sin_family = PF_INET; // address format IPV6
	servSocket.sin_port = htons(getPort()); // convert port
	servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) // server socket non blocking
		stop(errno);
	if (bind(sock, reinterpret_cast<struct sockaddr*>(&servSocket), sizeof(servSocket)) == -1)
		stop(errno);
	if (listen(sock, SOMAXCONN) == -1) // SOMAXCONN = max value
		stop(errno);
	initSupportedCommands();
}

void		Server::initSupportedCommands()
{
	_cmdList.push_back(Command("PASS",		1, &pass));
	_cmdList.push_back(Command("NICK",		1, &nick));
	_cmdList.push_back(Command("USER",		4, &user));
	_cmdList.push_back(Command("OPER",		2, &oper));
	_cmdList.push_back(Command("QUIT",		0, &quit));
	_cmdList.push_back(Command("JOIN",		1, &join));
	_cmdList.push_back(Command("PART",		1, &part));
	_cmdList.push_back(Command("MODE",		1, &mode));
	_cmdList.push_back(Command("TOPIC",		1, &topic));
	_cmdList.push_back(Command("NAMES",		0, &names));
	_cmdList.push_back(Command("LIST",		0, &list));
	_cmdList.push_back(Command("KICK",		2, &kick));
	_cmdList.push_back(Command("VERSION",	0, &version));
	_cmdList.push_back(Command("STATS",		0, &stats));
	_cmdList.push_back(Command("TIME",		0, &time));
	_cmdList.push_back(Command("ADMIN",		0, &admin));
	_cmdList.push_back(Command("INFO",		0, &info));
	_cmdList.push_back(Command("PRIVMSG",	2, &privmsg));
	_cmdList.push_back(Command("NOTICE",	2, &notice));
	_cmdList.push_back(Command("WHO",		0, &who));
	_cmdList.push_back(Command("WHOIS",		1, &whois));
	_cmdList.push_back(Command("WHOWAS",	1, &whowas));
	_cmdList.push_back(Command("KILL",		2, &kill));
	_cmdList.push_back(Command("PING",		1, &ping));
	_cmdList.push_back(Command("PONG",		1, &pong));
	_cmdList.push_back(Command("ERROR",		1, &error));
}

//~~ DESTRUCTOR

Server::~Server() {}

//~~ ACCESSOR

int	Server::getPort() const
{
	return _port;
}

std::string	Server::getPassword() const
{
	return _password;
}

//~~ METHODS

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock);
	
	clientsBySock.insert(std::make_pair(sock, newClient));
}

void	Server::removeClient(Client *src, std::vector<struct pollfd>::iterator it)
{
	std::cout << "Someone is disconnecting : " << it->fd << std::endl; // test
	oldClients.insert(std::make_pair(src->getNickname(), src));
	clientsByName.erase(src->getNickname());
	clientsBySock.erase(src->getSock());
	close(it->fd);
	fds.erase(it);
}

void	Server::sendMessages()
{
	int		ret;
	Client	*client;

	for (std::vector<struct pollfd>::iterator it = fds.begin() + 1; it < fds.end(); it++)
	{
		client = clientsBySock.find(it->fd)->second;
		if (client->hasOutput()) // request on this socket
		{
			// ret = send(it->fd, client->getOutputBuffer(), 10, 0);
			ret = send(it->fd, "Response.\n", 10, 0); // test
			if (ret < 0 && errno == ECONNRESET) // deconnexion
				removeClient(client, it);
		}
	}
}

void		Server::executeRequest(std::string cmdLine, Client* sender)
{
	size_t						i;
	std::vector<std::string>	cmdArgs;
	Command*					cmd = NULL;


	for (std::vector<Command>::iterator it = _cmdList.begin(); it != _cmdList.end(); it++)
		if (!cmdArgs.front().compare(it->getName()))
			*cmd = *it;
	if (cmd == NULL)
	{
		// sender->addToOutputBuffer(ERRUNKNOWNCOMMAND(cmd));
		return;
	}
	for (i = cmdLine.find_first_of(' '); i != std::string::npos; i = cmdLine.find_first_of(' '))
	{
		cmdArgs.push_back(cmdLine.substr(0, i));
		cmdLine.erase(0, i);
	}
	cmdArgs.push_back(cmdLine);
	if (cmdArgs.size() < cmd->getMinArg())
	{
		// sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(cmd));
		return;
	}
	cmd->execute(cmdArgs, sender, this);
}

// vérifier si la commande existe
// vérifier si elle a le nombre d'arguments minimal requis
// appeler la fonction d'exécution de la commande

void	Server::receiveMessages()
{
	int 	ret;
	char	*buf;
	Client	*client;

	for (std::vector<struct pollfd>::iterator it = fds.begin() + 1; it < fds.end(); it++)
	{
		client = clientsBySock.find(it->fd)->second;
		buf = client->getInputBuffer();
		if ((it->revents | POLLHUP) == it->revents) // deconnexion
			removeClient(client, it);
		else if (it->revents == POLLIN)
		{
			ret = recv(it->fd, buf, MAX_MESSAGE_LENGTH, 0);
			if (ret > 0)
			{
				buf[ret] = '\0';
				std::cout << "Message from socket : " << it->fd << std::endl; // test
				// TODO: execute() -> fct membre du client : parsing, exec and keep write in buffers
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
		socklen_t addrlen = sizeof(servSocket);
		ret = accept(fds[0].fd, reinterpret_cast<struct sockaddr*>(&servSocket), &addrlen);
		if (ret > 0)
		{
			std::cout << "Someone is connecting: " << ret << std::endl; // test
			tmp.fd = ret;
			tmp.events = POLLIN;
			fcntl(tmp.fd, F_SETFL, O_NONBLOCK); // client socket non blocking
			fds.push_back(tmp);
			addClient(ret);
		}
	} while (ret > 0);
}

void	Server::run()
{
	int	numberSockets; 
	
	online = true;
	while (online)
	{
		numberSockets = poll(&fds[0], fds.size(), 0); // return the number of socket with request and fill pollfd
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

	std::for_each(fds.begin(), fds.end(), closeFd);
	fds.clear();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
		delete it->second;
	channels.clear();
	for (std::map<int, Client*>::iterator it = clientsBySock.begin(); it != clientsBySock.end(); ++it)
		delete it->second;
	clientsBySock.clear();
	for (std::map<std::string, Client*>::iterator it = oldClients.begin(); it != oldClients.end(); ++it)
		delete it->second;
   	oldClients.clear();
	exit(status);
}