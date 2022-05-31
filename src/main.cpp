#include "irc.hpp"

void	cleanExit(t_env *irc, int exitStatue) // Plus cohérant si la variable s'appelle simplement `status`
{
	size_t	i = 0;

	std::cout << "Clean exit with statue : " << exitStatue << std::endl;	//	Je verrais plus un truc comme
																			//	if (status)
																			//	{
																			//		envoyer des messages comme quoi le serv crash aux clients
																			//		std::cerr << SERVER_NAME << "server exited with code: " << exitStatue << std::endl;
																			//	}
																			//	else
																			//	{
																			//		envoyer des messages comme quoi le serv ferme aux clients
																			//		std::cout << SERVER_NAME << "server closed" << std::endl;
																			//	}
	delete irc->serv;
	while (i < irc->fds.size()) // Utilise un itérateur et pas un size_t dans les container, c'est plus propre ET SURTOUT voit si tu peux utiliser la fonction `for_each()`
	{
		close(irc->fds[i].fd);
		i++;
	}
	irc->fds.clear();
	// TODO: clean all maps (clients, disconnectClients, channels) and free // appeler clear ne suffira pas, il faudra d'abord free manuellement tout les pointeurs avec `delete` en utilisant `for_each()`
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
  	if (setsockopt(irc->serv->sock, SOL_SOCKET,  SO_REUSEADDR, (char *)&sockopt, sizeof(sockopt)) < 0) // permit to reuse server address after close (protect bind from crash)	//	Les deux derniers arguments ne vont pas.
	  																																											//	Le premier est une chaine de caractères non `NULL-terminated` contenant au moins 1 caractère.
																																												//	Donc une chaine dont on ne connait pas la taille mais avec une taille voulue de 2 octets (l'un pour le premier caractère de la table ascii (int sockfd = 1) et l'autre pour le '\0').
																																												//	Tandis que le deuxième argument est la taille d'un int soit 8 octets.
																																												//	Je pense que tu ne devrais pas cast le pointeur sur `int` en pointeur sur `char`.
																																												//	Qui plus est tu peux remplacer les deux derniers arguments par ça : (&(int){1}, sizeof(int))
																																												//	Afin d'éviter d'instancier la variable `sockopt` (plus concis mais moins propre imho)
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
		errno = EINVAL;	// Pense à include le header <cerrno>
		perror(SERV_NAME);  // Enlève ce perror() pour le rajouter que dans le main
		return EXIT_FAILURE;
	}
	irc->serv = new Server(tmp, av[2]);
	if (!irc->serv) // Tu peux enlever la protections ici car `new` ne retourne jamais `NULL`
		return EXIT_FAILURE; // Par contre tu peux toujours `catch` l'exception `throw()` par `new` pour que notre code n'`abort` pas
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_env	irc;

	if (initServ(ac, av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (createServSocket(&irc) == EXIT_FAILURE) // Tu devrais déplacer cette partie là dans `initServ()` pour plus de cohérance et pour que le main soit plus clair
	{
		// Tu devrais rajouter perror(SERV_NAME);
		if (irc.serv->sock > 0)
			close(irc.serv->sock);
		delete irc.serv;
		return EXIT_FAILURE;
	}
	if (mainLoop(&irc) == EXIT_FAILURE) // Si mainLoop renvoie le status de retour dans ce cas utilise `cleanExit(&irc, mainLoop(&irc));`
		cleanExit(&irc, EXIT_FAILURE);
	cleanExit(&irc, EXIT_SUCCESS);
}
