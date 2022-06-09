#include "ircserv.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << USAGE << std::endl;
		return EXIT_FAILURE;
	}
	
	int serverPort = std::atoi(av[1]);

	if (serverPort < 0 || serverPort > MAX_PORT || !av[2][0])
	{
		errno = EINVAL;
		std::perror(SERV_NAME);
		return EXIT_FAILURE;
	}

	Server server(serverPort, av[2]);
	server.run();
}
