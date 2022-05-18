#include "irc.hpp"
#include "Server.hpp"

int	main_loop(t_env *irc)
{
	int numberSockets;
	int i = 0;

	while (irc->serv)
	{
		numberSockets = poll(--, --, NULL); // récupère le nombre de socket/client qui a une requête, NULL = non bloquant
		if (numberSockets == -1)
			return EXIT_FAILURE;
		while (numberSockets > 0)
		{
			// si fds.revents != POLLIN --> error
			// si i = sock du server
				// boucle d'accept(irc->serv.sock, --, --); --> if return = EWOULDBLOCK ou entier positif, ok, sinon EXIT_FAILURE
				// recv(j, buf, buflen, 0);
				// parsing, exec and keep write in buffers
				// send(j, buf, buflen, 0);
			// sinon i = sock d'un client
				// numberSockets--;
			i++;
		}
	}
	return EXIT_SUCCESS;
}

int	createServSocket(t_env *irc)
{
	irc->serv.sock = socket(PF_INET, SOCK_STREAM, irc->pe->p_proto); // settings TCP
	if (irc->serv.sock == -1)
		return EXIT_FAILURE;
	irc->servSocket.sin_family = PF_INET; // address format IPV6
	irc->servSocket.sin_port = htons(irc->serv->getPort()); // convert port
	irc->servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (bind(irc->serv.sock, (struct sockaddr*)&irc->servSocket, sizeof(irc->servSocket)) == -1)
		return EXIT_FAILURE;
	if (listen(irc->serv.sock, 0) == -1)
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
	if ((irc.pe = getprotobyname(PROTOCOL)) == NULL)
		return EXIT_FAILURE; // + free serv
	if (createServSocket(&irc) == EXIT_FAILURE)
		return EXIT_FAILURE; // + free serv
	if (main_loop(&irc) == EXIT_FAILURE)
		return EXIT_FAILURE; // + free serv, fd
	// free serv, fd
	return EXIT_SUCCESS;
}
