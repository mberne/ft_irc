#include "Server.hpp"

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
}

Server::~Server() {}

int	Server::getPort() const
{
	return _port;
}

std::string	Server::getPassword() const
{
	return _password;
}

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock);
	
	clientsBySock.insert(std::pair<int, Client *>(sock, newClient));
}


void	Server::removeClient(Client *src, std::vector<struct pollfd>::iterator it)
{
	std::cout << "Someone is disconnecting : " << it->fd << std::endl; // test
	oldClients.insert(std::pair<std::string, Client *>(src->getNickname(), src));
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
		// if (client->hasOutput()) // request on this socket
		if (it->revents != 0) // request on this socket
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