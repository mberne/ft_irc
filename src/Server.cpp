#include "Server.hpp"

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
}

Server::~Server() {}

std::string	Server::getStartTime() const
{
	struct tm *timeinfo;

	timeinfo = localtime(&_startTime);
	return asctime(timeinfo);
}

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock);
	
	_clientsBySock.insert(std::pair<int, Client *>(sock, newClient));
}


void	Server::removeClient(Client *src, std::vector<struct pollfd>::iterator it)
{
	std::cout << "Someone is disconnecting : " << it->fd << std::endl; // test
	_oldClients.insert(std::pair<std::string, Client *>(src->getNickname(), src));
	_clientsByName.erase(src->getNickname());
	_clientsBySock.erase(src->getSock());
	close(it->fd);
	_fds.erase(it);
}

void	Server::sendMessages()
{
	int		ret;
	Client	*client;

	for (std::vector<struct pollfd>::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
		if (client->hasOutput()) // request on this socket
		{
			// ret = send(it->fd, client->getOutputBuffer(), 10, 0);
			ret = send(it->fd, "Response.\n", 10, 0); // test
			if (ret < 0 && errno == ECONNRESET) // deconnexion
				removeClient(client, it);
		}
	}
}

void	Server::receiveMessages()
{
	int 	ret;
	char	*buf;
	Client	*client;

	for (std::vector<struct pollfd>::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
	{
		client = _clientsBySock.find(it->fd)->second;
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
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	for (std::map<int, Client*>::iterator it = _clientsBySock.begin(); it != _clientsBySock.end(); ++it)
		delete it->second;
	_clientsBySock.clear();
	for (std::map<std::string, Client*>::iterator it = _oldClients.begin(); it != _oldClients.end(); ++it)
		delete it->second;
   	_oldClients.clear();
	exit(status);
}