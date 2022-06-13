#include "ircserv.hpp"

void	pass(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 1)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (sender->isRegistered())
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname(), cmd[0]));
	else if (!cmd[1].compare(serv->getPassword()))
	{
		sender->setPassword(0);
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname(), cmd[0]));
	}
	else
		sender->setPassword(1);
}

void	nick(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	user(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	oper(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	quit(std::vector<std::string> cmd, Client* sender, Server* serv) // mberne
{
	(void)cmd; (void)sender; (void)serv;
}
