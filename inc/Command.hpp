#ifndef COMMAND_H
# define COMMAND_H

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "Client.hpp"

class Command
{
	public:

		Command(std::string cmdLine, Client* sender);
		~Command(void);

		static void		initSupportedCommands();

	private:

		std::string							_cmd;
		std::vector<std::string>			_args;
		Client*								_sender;

		static std::map<std::string, std::pair<int, void (*)(void)> >	_commands;

		void	pass(void);
		void	nick(void);
		void	user(void);
		void	oper(void);
		void	quit(void);
		void	join(void);
		void	part(void);
		void	mode(void);
		void	topic(void);
		void	names(void);
		void	list(void);
		void	kick(void);
		void	version(void);
		void	stats(void);
		void	time(void);
		void	admin(void);
		void	info(void);
		void	privmsg(void);
		void	notice(void);
		void	who(void);
		void	whois(void);
		void	whowas(void);
		void	kill(void);
		void	ping(void);
		void	pong(void);
		void	error(void);

};

#endif //~~ COMMAND_H
