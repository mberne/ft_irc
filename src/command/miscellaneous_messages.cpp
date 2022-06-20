#include "ircserv.hpp"

void	irc_version(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() > 1 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_VERSION(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ISUPPORT(sender->getNickname()));
	}
}

void	irc_stats(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else
	{
		if (cmd[1].compare(SERV_NAME))
			sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
		else
		{
			sender->addToOutputBuffer(RPL_STATSUPTIME(sender->getNickname(), serv));
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname()));
		}
	}
}

void	irc_time(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
	else
		sender->addToOutputBuffer(RPL_TIME(sender->getNickname(), serv->getCurrentTime()));
}

void	irc_admin(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_ADMINME(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC1(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC2(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINEMAIL(sender->getNickname()));
	}
}

void	irc_info(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
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

void	irc_lusers(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd;
	sender->addToOutputBuffer(RPL_LUSERCLIENT(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSEROP(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERUNKNOWN(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERCHANNELS(sender->getNickname(), serv));
}

void	irc_motd(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() == 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_MOTDSTART(sender->getNickname()));
		sender->addToOutputBuffer(RPL_MOTD(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ENDOFMOTD(sender->getNickname()));
	}
}
