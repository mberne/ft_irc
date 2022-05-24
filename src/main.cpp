#include "irc.hpp"
#include "Server.hpp"
#include "mainProcess.cpp"

int	createServSocket(t_env *irc)
{
	irc->serv->sock = socket(PF_INET, SOCK_STREAM, irc->pe->p_proto); // settings TCP
	if (irc->serv->sock == -1)
		return EXIT_FAILURE;
	irc->servSocket.sin_family = PF_INET; // address format IPV6
	irc->servSocket.sin_port = htons(irc->serv->getPort()); // convert port
	irc->servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (fcntl(irc->serv->sock, F_SETFL, O_NONBLOCK)) // server fd non blocking
		return EXIT_FAILURE;
	if (bind(irc->serv->sock, (struct sockaddr*)&irc->servSocket, sizeof(irc->servSocket)) == -1)
		return EXIT_FAILURE;
	// TOFIX: static_cast<struct sockaddr*>(&irc->servSocket) ??
	if (listen(irc->serv->sock, SOMAXCONN) == -1) // TOFIX : SOMAXCONN = max value
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

void	cleanExit(t_env *irc)
{
	size_t	i = 0;

	delete irc->serv;
	delete irc->pe;
	while (i < irc->fds.size())
	{
		close(irc->fds[i].fd);
		i++;
	}
	irc->fds.clear();
	// clean all maps (clients, disconnectClients, channels)
	exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
	t_env	irc;

	if (initServ(ac, av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if ((irc.pe = getprotobyname(PROTOCOL)) == NULL)
	{
		delete irc.serv;
		return EXIT_FAILURE;
	}
	if (createServSocket(&irc) == EXIT_FAILURE)
	{
		delete irc.serv;
		delete irc.pe;
		if (irc.serv->sock > 0)
			close(irc.serv->sock);
		return EXIT_FAILURE;
	}
	if (mainLoop(&irc) == EXIT_FAILURE)
		cleanExit(&irc);
	return EXIT_SUCCESS;
}
