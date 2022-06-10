#include "ircserv.hpp"

void	version(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "VERSION", cmd[0]));
	else
		sender->addToOutputBuffer(RPL_VERSION(sender->getNickname()));
}

void	stats(std::vector<std::string> cmd, Client* sender, Server* serv) // needtofix
{
	if (!cmd[2].empty() && !cmd[2].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[0]));
	else if (cmd[2].empty())
	{
		if (cmd[1].empty())
		{
			sender->addToOutputBuffer(RPL_STATSUPTIME(sender->getNickname(), serv));
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), ""));
		}
		else if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME) && cmd[1].size() == 1)
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), cmd[1]));
		else if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME) && cmd[1].size() != 1)
			sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[0]));
	}
}

void	time(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "TIME", cmd[0]));
	else
		sender->addToOutputBuffer(RPL_TIME(sender->getNickname(), serv->currentTime()));
}

void	admin(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "ADMIN", cmd[0]));
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
	if (!cmd[1].empty() && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "INFO", cmd[0]));
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
