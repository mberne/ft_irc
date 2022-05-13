#include "irc.hpp"
#include "Server.hpp"

void	print_error(std::string s)
{
	std::cout << s << std::endl;
}

int parsing(int ac, char **av, t_struct *irc)
{
	if (ac != 3)
	{
		print_error("Two arguments needed : <port> <password>.");
		return EXIT_FAILURE;
	}
	int tmp = atoi(av[1]);
	if (tmp < 0 || tmp > INT32_MAX)
	{
		print_error("<port> is not an int.");
		return EXIT_FAILURE;
	}
	if (!av[2][0])
	{
		print_error("<password> is empty.");
		return EXIT_FAILURE;
	}
	irc->serv = Server server(tmp, av[2]);
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_struct	*irc;

	if (!parsing(ac, av, &irc))
		return (EXIT_FAILURE);
	irc->pe = getprotobyname("tcp");
	if (!irc->pe)
		return (EXIT_FAILURE);

	//

	return (EXIT_SUCCESS);
}
