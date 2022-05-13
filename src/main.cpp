#include "irc.hpp"
#include "Server.hpp"

// free le serveur

int initServ(int ac, char **av, t_env *irc)
{
	if (ac != 3)
	{
		std::cout << USAGE << std::endl;
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
	if ((irc.pe = getprotobyname(PROTOCOL)) == NULL);
		return EXIT_FAILURE;

	//

	return EXIT_SUCCESS;
}
