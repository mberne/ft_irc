#include "ircserv.hpp"

void	irc_who(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	bool	option = 0;

	if ((cmd.size() == 2 && cmd[1].size() == 1 && !cmd[1].compare("o")) || (cmd.size() == 3 && cmd[2].size() == 1 && !cmd[2].compare("o")))
		option = 1;
	if ((cmd.size() == 2 && option) || cmd.size() == 1)
	{
		for (std::map<std::string, Client*>::iterator it = serv->getAllClients().begin(); it != serv->getAllClients().end(); it++)
			if (!it->second->hasMod(CLIENT_FLAG_I) && (!option || it->second->hasMod(CLIENT_FLAG_O)))
				sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), it->second));
	}
	else if ((cmd.size() == 2 && !option) || cmd.size() > 2)
	{
		if (serv->getClient(cmd[1]) && !serv->getClient(cmd[1])->hasMod(CLIENT_FLAG_I) && (!option || serv->getClient(cmd[1])->hasMod(CLIENT_FLAG_O)))
			sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), serv->getClient(cmd[1])));
		else if (serv->getChannel(cmd[1]))
			for (std::map<std::string, Client*>::iterator it = serv->getChannel(cmd[1])->getAllClients().begin(); it != serv->getChannel(cmd[1])->getAllClients().end(); it++)
				sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), it->second));
	}
	sender->addToOutputBuffer(RPL_ENDOFWHO(sender->getNickname(), cmd[1]));
}

void	irc_whois(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	int	mask;
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname(), "WHOIS"));
	else
	{
		if (cmd.size() == 2)
			mask = 1;
		else
		{
			if (cmd[1].compare(SERV_NAME))
			{
				sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "WHOIS", cmd[1]));
				return ;
			}
			mask = 2;
		}
		if (serv->getClient(cmd[mask]))
		{
			sender->addToOutputBuffer(RPL_WHOISUSER(sender->getNickname(), serv->getClient(cmd[mask])));
			sender->addToOutputBuffer(RPL_WHOISSERVER(sender->getNickname(), cmd[mask]));
			sender->addToOutputBuffer(RPL_WHOISACTUALLY(sender->getNickname(),serv->getClient(cmd[mask])));
			sender->addToOutputBuffer(RPL_WHOISOPERATOR(sender->getNickname(), cmd[mask]));
			sender->addToOutputBuffer(RPL_WHOISIDLE(sender->getNickname(), serv->getClient(cmd[mask])));
			if (serv->getClient(cmd[mask])->showChannelList().size() > 0)
				sender->addToOutputBuffer(RPL_WHOISCHANNELS(sender->getNickname(), cmd[mask], serv->getClient(cmd[mask])));
			sender->addToOutputBuffer(RPL_ENDOFWHOIS(sender->getNickname(), cmd[mask]));
		}
		else
			ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[mask]);
	}
}

void	irc_whowas(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv; (void)sender; (void)cmd;
	// int count;
	// int	i = 0;

	// if (cmd.size() < 2)
	// 	sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname(), "WHOWAS"));
	// else
	// {
	// 	if (cmd.size() == 3)
	// 	{
	// 		count = std::stoi(cmd[2]);
	// 		if (count < 1)
	// 			count = INT32_MAX;
	// 	}
	// 	for (std::map<std::string, Client*>::iterator it = serv->getOldNickname().begin(); it != serv->getOldNickname().end(); it++)
	// 	{
	// 		if (!it->second->getNickname().compare(cmd[1]) && i < count)
	// 		{
	// 			sender->addToOutputBuffer(RPL_WHOWASUSER(sender->getNickname(), it->second));
	// 			sender->addToOutputBuffer(RPL_WHOISSERVER(sender->getNickname(), it->second->getNickname()));
	// 			sender->addToOutputBuffer(RPL_WHOISACTUALLY(sender->getNickname(), it->second));
	// 			i++;
	// 		}
	// 	}
	// 	if (!i)
	// 		sender->addToOutputBuffer(ERR_WASNOSUCHNICK(sender->getNickname(), cmd[0], cmd[1]));
	// 	sender->addToOutputBuffer(RPL_ENDOFWHOWAS(sender->getNickname(), cmd[1]));
	// }
}
