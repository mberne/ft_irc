#include "ircserv.hpp"

void	version(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() > 1 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "VERSION", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_VERSION(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ISUPPORT(sender->getNickname()));
	}
}

void	stats(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	// query pris en compte dans le code pour erreurs etc mais pas pour la réponse finale...
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), "STATS"));
	else if (cmd.size() == 2)
	{
		if (cmd[1].size() != 1 && cmd[1].compare(SERV_NAME))
			sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[1]));
		else if (cmd[1].size() == 1)
		{
			sender->addToOutputBuffer(RPL_STATSUPTIME(sender->getNickname(), serv));
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), cmd[1][0]));
		}
	}
	else
	{
		if (cmd[2].compare(SERV_NAME))
			sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[2]));
		else
		{
			sender->addToOutputBuffer(RPL_STATSUPTIME(sender->getNickname(), serv));
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), cmd[1][0]));
		}
	}

}

void	time(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "TIME", cmd[1]));
	else
		sender->addToOutputBuffer(RPL_TIME(sender->getNickname(), serv->currentTime()));
}

void	admin(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "ADMIN", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_ADMINME(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC1(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC2(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINEMAIL(sender->getNickname()));
	}
}

void	info(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "INFO", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_NAME));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_VERSION));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_ADMIN));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_ADMIN_EMAIL));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_INFO));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_LOC1));
		sender->addToOutputBuffer(RPL_INFO(sender->getNickname(), SERV_LOC2));
		sender->addToOutputBuffer(RPL_ENDOFINFO(sender->getNickname()));
	}
}

void	lusers(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd;
	sender->addToOutputBuffer(RPL_LUSERCLIENT(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSEROP(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERUNKNOWN(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERCHANNELS(sender->getNickname(), serv));
}

void	motd(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "VERSION", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_MOTDSTART(sender->getNickname()));
		sender->addToOutputBuffer(RPL_MOTD(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ENDOFMOTD(sender->getNickname()));
	}
}
