#include "irc.hpp"

void	cleanExit(t_env *irc, int exitStatue)
{
	size_t	i = 0;

	std::cout << "Clean exit with statue : " << exitStatue << std::endl;
	delete irc->serv;
	while (i < irc->fds.size())
	{
		close(irc->fds[i].fd);
		i++;
	}
	irc->fds.clear();
	// TODO: clean all maps (clients, disconnectClients, channels) and free
	// irc->serv->channels.clear();
	// irc->serv->clients.clear();
	// irc->serv->disconnectClients.clear();
	exit(exitStatue);
}

int	createServSocket(t_env *irc)
{
	struct protoent	*pe;
	int				sockopt = 1;

	if ((pe = getprotobyname(PROTOCOL)) == NULL)
		return EXIT_FAILURE;
	irc->serv->sock = socket(PF_INET, SOCK_STREAM, pe->p_proto); // settings TCP
	if (irc->serv->sock == -1)
		return EXIT_FAILURE;
  	if (setsockopt(irc->serv->sock, SOL_SOCKET,  SO_REUSEADDR, (char *)&sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)
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
	return EXIT_SUCCESS;
}

int initServ(int ac, char **av, t_env *irc)
{
	if (ac != 3)
	{
		std::cerr << USAGE << std::endl;
		return EXIT_FAILURE;
	}
	int tmp = std::atoi(av[1]);
	if (tmp < 0 || tmp > INT32_MAX || !av[2][0])
	{
		errno = EINVAL;
		perror(SERV_NAME); 
		return EXIT_FAILURE;
	}
	irc->serv = new Server(tmp, av[2]);
	if (!irc->serv)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_env	irc;

	if (initServ(ac, av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (createServSocket(&irc) == EXIT_FAILURE)
	{
		if (irc.serv->sock > 0)
			close(irc.serv->sock);
		delete irc.serv;
		return EXIT_FAILURE;
	}
	if (mainLoop(&irc) == EXIT_FAILURE)
		cleanExit(&irc, EXIT_FAILURE);
	cleanExit(&irc, EXIT_SUCCESS);
}
