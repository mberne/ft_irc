#include "ircserv.hpp"

void	stop_handler(int)
{
	Server::online = false;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << USAGE << std::endl;
		return EXIT_FAILURE;
	}
	
	int serverPort = std::atoi(av[1]);
	if (serverPort < 0 || serverPort > MAX_PORT || !av[2][0] || std::string(av[1]).find_first_not_of("0123456789") != std::string::npos)
	{
		errno = EINVAL;
		std::perror(SERV_NAME.c_str());
		return EXIT_FAILURE;
	}

	Server server(serverPort, av[2]);

	std::signal(SIGINT, &stop_handler);
	std::signal(SIGQUIT, &stop_handler);
	std::signal(SIGTERM, &stop_handler);

	try			{server.run();}
	catch(...)	{server.stop(errno);}
}
