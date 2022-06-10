#include "Command.hpp"

//~~ CONSTRUCTOR

Command::Command(std::string name, int minArg, void (*fct)(std::vector<std::string> cmd, Client* sender, Server* server)) : fct(fct), _name(name), _minArg(minArg) {}

//~~ DESTRUCTOR

Command::~Command() {}

//~~ ACCESSOR

std::string		Command::getName() const
{
	return (_name);	
}

size_t		Command::getMinArg() const
{
	return (_minArg);	
}
