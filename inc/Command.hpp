#ifndef COMMAND_H
# define COMMAND_H

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "Server.hpp"
# include "Client.hpp"

class Command
{
	public:

		Command(std::string name, int minArg, void (*fct)(std::vector<std::string> cmd, Client* sender, Server* server));
		~Command();

		std::string		getName() const;
		size_t			getMinArg() const;

		void 				(*fct)(std::vector<std::string> cmd, Client* sender, Server* server);	// A pointer to the function that execute the command

	private:

		std::string const	_name;																	// The command name
		size_t const		_minArg;																// The minimum number of arguments the command need
};

#endif //~~ COMMAND_H
