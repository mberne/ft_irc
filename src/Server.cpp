#include "Server.hpp"

bool Server::online = false;

//~~ CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), _startTime(std::time(NULL))
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
	_commands.insert(std::make_pair("INVITE", &irc_invite));
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
	_commands.insert(std::make_pair("PING", &irc_ping));
	_commands.insert(std::make_pair("PONG", &irc_pong));
}

//~~ DESTRUCTOR

Server::~Server() {}

//~~ SERVER

in_port_t	Server::getPort() const
{
	return _port;
}

std::string	Server::getPassword() const
{
	return _password;
}

std::string	Server::getStartTime() const
{
	std::string	time = asctime(localtime(&_startTime));
	size_t		pos = time.find("  ");

	time.erase(pos, 1); // Remove space
	time[time.size() - 1] = '\0'; // Remove '\n';
	return time;
}

std::string	Server::getCurrentTime() const
{
	time_t		currentTime = time(NULL);
	std::string	time = asctime(localtime(&currentTime));
	size_t		pos = time.find("  ");

	time.erase(pos, 1); // Remove space
	time.erase(time.size() - 1); // Remove '\n';
	return time;
}

void		Server::addLog(std::string message, mode_t type)
{
	std::string		logPrompt;
	switch (type)
	{
		case LOG_INFO:
			logPrompt = "[" + getCurrentTime() + "]: INFO:\t\t";
			break ;
		case LOG_LISTEN:
			logPrompt = "[" + getCurrentTime() + "]: LISTEN:\t\t";
			break ;
		case LOG_BROADCAST:
			logPrompt = "[" + getCurrentTime() + "]: BROADCAST:\t";
			break ;
		case LOG_MESSAGE:
			logPrompt = "[" + getCurrentTime() + "]: MESSAGE:\t";
			break ;
		case LOG_ERROR:
			logPrompt = "[" + getCurrentTime() + "]: ERROR:\t\t";
			break ;
	}

	size_t	start = 0;
	message.erase(remove(message.begin(), message.end(), '\r'), message.end());
	for (size_t i = message.find('\n'); i != std::string::npos; i = message.find('\n', start))
	{
		std::string		line = message.substr(start, i - start);

		_logFile << logPrompt + line << std::endl;
		start = i + sizeof(char);
	}
	if (message[message.size() - 1] != '\n')
		_logFile << logPrompt + message.substr(start, message.size() - start) << std::endl;
}

//~~ CLIENT

Client*		Server::getClient(std::string name) const
{
	if (_clientsByName.find(name) == _clientsByName.end())
		return (NULL);
	else
		return (_clientsByName.find(name)->second);
}

std::map<std::string, Client*>&		Server::getAllClients()
{
	return _clientsByName;
}

size_t	Server::getOpsNumber()
{
	size_t	count = 0;

	for (std::map<std::string, Client*>::iterator it = _clientsByName.begin(); it != _clientsByName.end(); it++)
		if (it->second->hasModes(CLIENT_FLAG_O))
			count++;
	return count;
}

size_t	Server::getNonRegisteredNumber()
{
	size_t	count = 0;

	for (std::map<std::string, Client*>::iterator it = _clientsByName.begin(); it != _clientsByName.end(); it++)
		if (!(it->second->isRegistered()))
			count++;
	return count;
}

void	Server::addOldNickname(Client* client)
{
	std::vector<std::string>	ident;
	ident.push_back(client->getNickname());
	ident.push_back(client->getUser());
	ident.push_back(client->getHost());
	ident.push_back(client->getRealName());

	if (_oldNicknames.size() >= OLD_CLIENT_LIMIT)
		_oldNicknames.pop_back();
	_oldNicknames.push_front(ident);
}

std::list<std::vector<std::string> >&	Server::getOldNicknames()
{
	return _oldNicknames;
}

void	Server::addClient(int sockfd)
{
	Client*		newClient = new Client(sockfd);
	
	newClient->setHost(inet_ntoa(_servSocket.sin_addr));
	_clientsBySock.insert(std::make_pair(sockfd, newClient));

	addLog("New connexion: " + newClient->getPrefix(), LOG_INFO);

	if (_clientsBySock.size() >= CLIENT_LIMIT)
	{
		addLog("Connexion closed: " + newClient->getPrefix(), LOG_INFO);
		irc_error(newClient, "Closing Link: " +  newClient->getHost() + " Server full");
		send(newClient->getSock(), newClient->getOutputBuffer(), strlen(newClient->getOutputBuffer()), 0);
		removeClient(newClient);
	}
}

void	Server::removeClient(Client *client)
{
	addLog("Connexion closed: " + client->getPrefix(), LOG_INFO);

	std::vector<struct pollfd>::iterator it = _fdList.begin();
	while (it->fd != client->getSock())
		it++;

	for (Channel* channel = client->getChannel(client->getLastChannelName()); channel != NULL; channel = client->getChannel(client->getLastChannelName()))
		client->leaveChannel(channel, this);
	addOldNickname(client);
	if (client->isRegistered() == true && _clientsByName.find(client->getNickname())->second == client)
		_clientsByName.erase(client->getNickname());
	_clientsBySock.erase(it->fd);
	delete client;
	close(it->fd);
	_fdList.erase(it);
}

void	Server::pingClient(Client* client)
{
	time_t	diff = time(NULL) - client->getLastCmdTime();

	if (diff > TIME_AFK)
	{
		if (!client->getIsPing())
		{
			client->addToOutputBuffer("PING :" + SERV_NAME);
			client->setIsPing(true);
		}
		if (diff > TIME_AFK + PING_TIME)
			irc_quit(vectorizator("QUIT", "Ping timeout:" + std::to_string(PING_TIME) + " seconds", ""), client, this);
	}
}

std::string		Server::showLonelyClientsList()
{
	std::string		clientList;

	for (std::map<std::string, Client*>::iterator it = _clientsByName.begin(); it != _clientsByName.end(); it++)
	{
		if (!it->second->hasModes(CLIENT_FLAG_I) && it->second->getNumberOfChannels() == 0)
		{
			if (it != _clientsByName.begin())
				clientList += " ";
			clientList += it->second->getNickname();
		}
	}
	return clientList;
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

Channel*	Server::addChannel(std::string name, Client* founder)
{
	addLog("New channel: " + name + " created by: " + founder->getPrefix(), LOG_INFO);

	Channel* channel = new Channel(name);

	_channels.insert(std::make_pair(name, channel));
	channel->addOperator(founder);
	return channel;
}

void	Server::removeChannel(Channel* channel)
{
	addLog("Channel deleted: " + channel->getName(), LOG_INFO);

	_channels.erase(channel->getName());
	delete channel;
}

//~~ SERVER MAIN

void	Server::run()
{
	int	socketsNumber; 
	
	_logFile.open((SERV_NAME + ".log").c_str() , std::ios_base::out | std::ios_base::trunc);
	if ( _logFile.is_open() == false )
		stop(errno);

	online = true;
	addLog(START_LOG, LOG_INFO);
	while (online)
	{
		socketsNumber = poll(&_fdList[0], _fdList.size(), 0); // return the number of socket with request and fill pollfd
		if (socketsNumber == -1)
			stop(errno);
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
	int				tmpErrno;

	do
	{
		socklen_t addrlen = sizeof(_servSocket);
		tmpErrno = errno;
		ret = accept(_fdList[0].fd, reinterpret_cast<struct sockaddr*>(&_servSocket), &addrlen);
		errno = tmpErrno;
		if (ret > 0)
		{
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

	for (std::vector<struct pollfd>::iterator it = _fdList.begin() + 1; it < _fdList.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if ((it->revents | POLLHUP) == it->revents) // deconnexion
			irc_quit(vectorizator("QUIT", "Remote host closed the connection", ""), client, this);
		else
		{
			if (it->revents == POLLIN)
			{
				ret = recv(it->fd, buf, TCP_MAXWIN, 0);
				if (ret > 0)
				{
					client->setLastCmdTime();
					buf[ret] = '\0';
					client->addToInputBuffer(buf);
					if (CRLF.compare(buf))
						addLog("from: " + client->getPrefix() + " to: :" + SERV_NAME + "\n" + buf, LOG_LISTEN);
					executeRequest(client);
				}
				else if (ret < 0)
					irc_quit(vectorizator("QUIT", "Remote host closed the connection", ""), client, this);
			}
			if (!client->isRegistered() && time(NULL) - client->getConnexionStartTime() > CONNEXION_TIME)
				irc_quit(vectorizator("QUIT", "Connection timed out", ""), client, this);
			pingClient(client);
		}
	}
}

void		Server::executeRequest(Client* sender)
{
	for (size_t i = sender->getInputBuffer().find(CRLF); i != std::string::npos; i = sender->getInputBuffer().find(CRLF))
	{
		size_t						start = sender->getInputBuffer().find_first_not_of(" ");
		std::string					cmdLine = sender->getInputBuffer().substr(start, i - start);
		std::vector<std::string>	cmdArgs;

		if (sender->getInputBuffer().find(CRLF) >= MESSAGE_LEN - CRLF.size())
			cmdLine.erase(MESSAGE_LEN, cmdLine.size() - MESSAGE_LEN);
		if (!cmdLine.empty())
		{
			if (cmdLine[0] == ':')
				cmdLine.erase(0, cmdLine.find_first_not_of(' ', cmdLine.find_first_of(' ', 0)));
			for (size_t endOfArg = cmdLine.find_first_of(' '); endOfArg != std::string::npos; endOfArg = cmdLine.find_first_of(' '))
			{
				if (cmdLine[0] == ':')
					break;
				cmdArgs.push_back(cmdLine.substr(0, endOfArg));
				cmdLine.erase(0, cmdLine.find_first_not_of(' ', endOfArg));
			}
			if (cmdLine[0] == ':')
				cmdArgs.push_back(cmdLine.substr(1, cmdLine.size() - 1));
			else if (!cmdLine.empty())
				cmdArgs.push_back(cmdLine);
			executeCommand(cmdArgs, sender);
		}
		sender->getInputBuffer().erase(0, i + CRLF.size());
	}
	if (sender->getInputBuffer().size() >= MESSAGE_LEN - CRLF.size())
	{
		sender->addToInputBuffer(CRLF.c_str());
		executeRequest(sender);
	}
}

void	Server::sendMessages()
{
	int		ret;
	Client	*client;

	for (std::vector<struct pollfd>::iterator it = _fdList.begin() + 1; it < _fdList.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if (client->hasOutput()) // request on this socket
		{
			ret = send(it->fd, client->getOutputBuffer(), strlen(client->getOutputBuffer()), 0);
			if (ret < 0 && errno == ECONNRESET) // deconnexion
				irc_quit(vectorizator("QUIT", "Remote host closed the connection", ""), client, this);
			addLog("from: :" + SERV_NAME + " to: " + client->getPrefix() + "\n" + client->getOutputBuffer(), LOG_BROADCAST);
			client->clearOutputBuffer();
		}
	}
}

void	closeFd(struct pollfd rhs) {close(rhs.fd);}

void	Server::stop(int status)
{
	for (std::map<int, Client*>::iterator it = _clientsBySock.begin(); it != _clientsBySock.end(); it = _clientsBySock.begin())
	{
		irc_error(it->second, "Closing Link: " +  it->second->getHost() + " Server is shuting down :" + (status == EXIT_SUCCESS ? "Closed by host" : "Fatal error"));
		send(it->second->getSock(), it->second->getOutputBuffer(), strlen(it->second->getOutputBuffer()), 0);
		removeClient(it->second);
	}

	if (status)
		addLog("Server exited with code: " + std::to_string(status) + " " + SERV_NAME + ": " + strerror(status), LOG_ERROR);
	else
		addLog("Server exited with code: " + std::to_string(status) + " " + SERV_NAME + ": Closed by host", LOG_INFO);
	char	buf[PATH_MAX];
	(status ? std::cerr : std::cout) << "Server exited with code: " << status << "\nLogs available at: " << getcwd(buf, PATH_MAX) << "/" << SERV_NAME << ".log" << std::endl;
	_logFile.close();
	
	std::for_each(_fdList.begin(), _fdList.end(), closeFd);
	_fdList.clear();
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete it->second;
	_channels.clear();
	for (std::map<int, Client*>::iterator it = _clientsBySock.begin(); it != _clientsBySock.end(); it++)
		delete it->second;
	_clientsBySock.clear();
	for (std::list< std::vector<std::string> >::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
		it->clear();
   	_oldNicknames.clear();

	std::signal(SIGINT, SIG_DFL);
	std::signal(SIGQUIT, SIG_DFL);
	std::signal(SIGTERM, SIG_DFL);
	exit(status);
}

//~~ SERVER UTILS

void		Server::executeCommand(std::vector<std::string>	cmdArgs, Client* sender)
{
	std::map<std::string, command_t>::iterator it = _commands.find(cmdArgs.front());
	if (it == _commands.end())
		sender->addToOutputBuffer(ERR_UNKNOWNCOMMAND(sender->getNickname(), cmdArgs.front()));
	else if (sender->isRegistered() == false && it->first.compare("USER") && it->first.compare("PASS") && it->first.compare("NICK") && it->first.compare("QUIT"))
		sender->addToOutputBuffer(ERR_NOTREGISTERED(sender->getNickname()));
	else if (sender->isRegistered() == false)
	{
		command_t fct = it->second;
		fct(cmdArgs, sender, this);
		if (_clientsByName.find(sender->getNickname()) == _clientsByName.end() && sender->isRegistered() == true)
		{
			_clientsByName.insert(std::make_pair(sender->getNickname(), sender));
			sendWelcome(sender);
		}
		else if (_clientsByName.find(sender->getNickname()) != _clientsByName.end() && _clientsByName.find(sender->getNickname())->second != sender)
			irc_quit(vectorizator("QUIT", "Nickname overridden", ""), sender, this);
	}
	else
	{
		command_t fct = it->second;
		fct(cmdArgs, sender, this);
	}
	
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
