#include "ircserv.hpp"

void	who(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
	// manque -o
	// (void)serv;
	// if (cmd.size() < 2)
	// 	sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), "WHO"));
	// else
	// {
	// 	if (!cmd[1].compare(SERV_NAME))
	// 		// sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), SERV_NAME));
	// 	else if (serv->getClient(cmd[1]))
	// 		// sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), serv->getClient(cmd[1])));
	// 	else if (serv->getChannel(cmd[1]))
	// 		// sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), serv->getChannel(cmd[1])));
	// 	sender->addToOutputBuffer(RPL_ENDOFWHO(sender->getNickname(), cmd[1]));
	// }
}

void	whois(std::vector<std::string> cmd, Client* sender, Server* serv)
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
			sender->addToOutputBuffer(RPL_WHOISOPERATOR(sender->getNickname(), cmd[mask]));
			// sender->addToOutputBuffer(RPL_WHOISIDLE(sender->getNickname(), serv->getOldClient(cmd[mask]))); //getOldClient //getSeconds // getSignon
			sender->addToOutputBuffer(RPL_WHOISCHANNELS(sender->getNickname(), cmd[mask], serv->getClient(cmd[mask])));
			sender->addToOutputBuffer(RPL_ENDOFWHOIS(sender->getNickname(), cmd[mask]));
		}
		else
			ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[mask]);
	}
}

void	whowas(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	// (void)serv;
	// int count;
	// int i;

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
	// 	if (getOldNickname(cmd[1]))
	// 	{
	// 		while (i < count)
	// 		{
	// 			// RPL_WHOWASUSER(sender->getNickname(), ?);
	// 			i++;
	// 		}
	// 	}
	// 	else
	// 		sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[1]));
	// 	sender->addToOutputBuffer(RPL_ENDOFWHOWAS(sender->getNickname(), cmd[1]));
	}
}
