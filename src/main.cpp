#include "irc.hpp"
#include "Server.hpp"

int	createServSocket(t_env *irc)
{
	int	sock = socket(AF_INET, SOCK_STREAM, NULL);
	irc->servSocket.sin_family = AF_INET; // TCP
	irc->servSocket.sin_port = htons(irc->serv->getPort()); // Convert port
	irc->servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (bind(sock, (sockaddr*)&irc->servSocket, sizeof(irc->servSocket)) == -1)
		return EXIT_FAILURE;
	if (listen(sock, SOMAXCONN) == -1) // value max for second arg ?
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
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_env	irc;

	if (initServ(ac, av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if ((irc.pe = getprotobyname(PROTOCOL)) == NULL)
		return EXIT_FAILURE;
	if (createServSocket(&irc) == EXIT_FAILURE)
		return EXIT_FAILURE;

	// boucle inf tant que le serveur est online
	// poll( -- , -- , NULL);

	// parsing commande
	// exec commande
	return EXIT_SUCCESS;
}
