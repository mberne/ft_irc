#include "ircserv.hpp"

void	irc_kill(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 3)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (!sender->hasModes(CLIENT_FLAG_O))
		sender->addToOutputBuffer(ERR_NOPRIVILEGES(sender->getNickname()));
	else
	{
		Client *client = serv->getClient(cmd[1]);
		if (!client)
			sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[1]));
		else
			irc_quit(vectorizator("QUIT", "Closing Link: " + SERV_NAME + " (Killed (" + sender->getNickname() + " (" + cmd[2] + ")))"), client, serv);
	}
}

void	irc_pong(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)serv;
	sender->setIsPing(0);
}

void	irc_ping(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)serv;
	sender->addToOutputBuffer("PONG");
}

void	irc_error(Client* sender, std::string reason)
{
	sender->addToOutputBuffer("ERROR :" + reason);
}
