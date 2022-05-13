#include "irc.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
		return 1;
	Server	server(av[1], av[2]);
	return 0;
}
