#include "Command.hpp"



//~~ CONSTRUCTOR

Command::Command(std::string cmdLine, Client* sender)
{
	// parsing
}

void		Command::initSupportedCommands()
{
	_commands["PASS"]		= std::pair<int, void (*)(void)>(1, *pass);
	_commands["NICK"]		= std::pair<int, void (*)(void)>(1, *nick);
	_commands["USER"]		= std::pair<int, void (*)(void)>(4, *user);
	_commands["OPER"]		= std::pair<int, void (*)(void)>(2, *oper);
	_commands["QUIT"]		= std::pair<int, void (*)(void)>(0, *quit);
	_commands["JOIN"]		= std::pair<int, void (*)(void)>(1, *join);
	_commands["PART"]		= std::pair<int, void (*)(void)>(1, *part);
	_commands["MODE"]		= std::pair<int, void (*)(void)>(1, *mode);
	_commands["TOPIC"]		= std::pair<int, void (*)(void)>(1, *topic);
	_commands["NAMES"]		= std::pair<int, void (*)(void)>(0, *names);
	_commands["LIST"]		= std::pair<int, void (*)(void)>(0, *list);
	_commands["KICK"]		= std::pair<int, void (*)(void)>(2, *kick);
	_commands["VERSION"]	= std::pair<int, void (*)(void)>(0, *version);
	_commands["STATS"]		= std::pair<int, void (*)(void)>(0, *stats);
	_commands["TIME"]		= std::pair<int, void (*)(void)>(0, *time);
	_commands["ADMIN"]		= std::pair<int, void (*)(void)>(0, *admin);
	_commands["INFO"]		= std::pair<int, void (*)(void)>(0, *info);
	_commands["PRIVMSG"]	= std::pair<int, void (*)(void)>(2, *privmsg);
	_commands["NOTICE"]		= std::pair<int, void (*)(void)>(2, *notice);
	_commands["WHO"]		= std::pair<int, void (*)(void)>(0, *who);
	_commands["WHOIS"]		= std::pair<int, void (*)(void)>(1, *whois);
	_commands["WHOWAS"]		= std::pair<int, void (*)(void)>(1, *whowas);
	_commands["KILL"]		= std::pair<int, void (*)(void)>(2, *kill);
	_commands["PING"]		= std::pair<int, void (*)(void)>(1, *ping);
	_commands["PONG"]		= std::pair<int, void (*)(void)>(1, *pong);
	_commands["ERROR"]		= std::pair<int, void (*)(void)>(1, *error);
}

//~~ DESTRUCTOR

Command::~Command(void) {}

//~~ OVERLOAD

//~~ ACCESSOR

//~~ METHODS
