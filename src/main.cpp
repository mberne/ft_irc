#include "irc.hpp"

void	closeFd(struct pollfd rhs)
{
	close(rhs.fd);
}

void	deleter(std::pair<std::string, Client*> pair)
{
	delete(pair.second);
}

void	deleter(std::pair<std::string, Channel*> pair)
{
	delete(pair.second);
}

void	cleanExit(t_env *irc, int status)
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
	std::for_each(irc->fds.begin(), irc->fds.end(), closeFd);
	irc->fds.clear();
	for (std::map<std::string, Channel*>::iterator it = irc->serv->channels.begin(); it != irc->serv->channels.end(); ++it)
		delete it->second;
	irc->serv->channels.clear();
	for (std::map<int, Client*>::iterator it = irc->serv->clientsBySock.begin(); it != irc->serv->clientsBySock.end(); ++it)
		delete it->second;
	irc->serv->clientsBySock.clear();
	for (std::map<std::string, Client*>::iterator it = irc->serv->oldClients.begin(); it != irc->serv->oldClients.end(); ++it)
		delete it->second;
   	irc->serv->oldClients.clear();
	delete irc->serv;
	exit(status);
}

int	createServSocket(t_env *irc)
{
	struct protoent	*pe;
	struct pollfd	tmp;
	int				sockopt = 1;

	if ((pe = getprotobyname("tcp")) == NULL)
		return EXIT_FAILURE;
	irc->serv->sock = socket(PF_INET, SOCK_STREAM, pe->p_proto); // settings TCP
	if (irc->serv->sock == -1)
		return EXIT_FAILURE;
  	if (setsockopt(irc->serv->sock, SOL_SOCKET,  SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)
		return EXIT_FAILURE;
	irc->servSocket.sin_family = PF_INET; // address format IPV6
	irc->servSocket.sin_port = htons(irc->serv->getPort()); // convert port
	irc->servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (fcntl(irc->serv->sock, F_SETFL, O_NONBLOCK) == -1) // server socket non blocking
		return EXIT_FAILURE;
	if (bind(irc->serv->sock, reinterpret_cast<struct sockaddr*>(&irc->servSocket), sizeof(irc->servSocket)) == -1)
		return EXIT_FAILURE;
	if (listen(irc->serv->sock, SOMAXCONN) == -1) // SOMAXCONN = max value
		return EXIT_FAILURE;
	tmp.fd = irc->serv->sock; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	irc->fds.push_back(tmp);
	return EXIT_SUCCESS;
}

int initServ(char **av, t_env *irc)
{	
	int tmp = std::atoi(av[1]);

	if (tmp < 0 || tmp > MAX_PORT || !av[2][0])
	{
		errno = EINVAL;
		perror(SERV_NAME);
		return EXIT_FAILURE;
	}
	try
	{
		irc->serv = new Server(tmp, av[2]);
	}
	catch(const std::bad_alloc& ba)
	{
		std::cerr << SERV_NAME << ": " << ba.what() << std::endl;
		return EXIT_FAILURE;
	}
	if (createServSocket(irc) == EXIT_FAILURE)
	{
		perror(SERV_NAME);
		if (irc->serv->sock > 0)
			close(irc->serv->sock);
		delete irc->serv;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_env	irc;

	if (ac != 3)
	{
		std::cerr << USAGE << std::endl;
		return EXIT_FAILURE;
	}
	if (initServ(av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	cleanExit(&irc, mainLoop(&irc));
}
