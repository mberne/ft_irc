#include "ircserv.hpp"

void	irc_who(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	bool	showOnlyOperators = false;

	if ((cmd.size() > 2 && !cmd[2].compare("o")) || (cmd.size() == 2 && !cmd[1].compare("o")))
		showOnlyOperators = true;
	if (cmd.size() > 2 || (cmd.size() == 2 && showOnlyOperators == false))
	{
		Channel*	channel = serv->getChannel(cmd[1]);
		Client*		client = serv->getClient(cmd[1]);

		if (client && (!showOnlyOperators || client->hasModes(CLIENT_FLAG_O)))
			sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), client));
		else if (channel)
			for (std::map<std::string, Client*>::iterator it = channel->getAllClients().begin(); it != channel->getAllClients().end(); it++)
				if ((!it->second->hasModes(CLIENT_FLAG_I) || channel->getClient(sender->getNickname()) != NULL) && (!showOnlyOperators || it->second->hasModes(CLIENT_FLAG_O)))
					sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), it->second));
	}
	else
	{
		for (std::map<std::string, Client*>::iterator it = serv->getAllClients().begin(); it != serv->getAllClients().end(); it++)
			if (!it->second->hasModes(CLIENT_FLAG_I) && (!showOnlyOperators || it->second->hasModes(CLIENT_FLAG_O)))
				sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), it->second));
	}
	sender->addToOutputBuffer(RPL_ENDOFWHO(sender->getNickname(), (cmd.size() == 1 ? "*" : cmd[1])));
}

void	irc_whois(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	int	mask = (cmd.size() < 3 ? 1 : 2);

	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname()));
	else if (cmd.size() > 2 && cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), cmd[1]));
	else if (serv->getClient(cmd[mask]))
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
		ERR_NOSUCHNICK(sender->getNickname(), cmd[mask]);
}

void	irc_whowas(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	size_t	count = (cmd.size() > 2 ? std::stoi(cmd[2]) : OLD_CLIENT_LIMIT);
	size_t	i = 0;

	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname()));
	else
	{
		for (std::list< std::vector<std::string> >::iterator it = serv->getOldNicknames().begin(); it != serv->getOldNicknames().end(); it++)
		{
			if (!(*it)[0].compare(cmd[1]) && i < count)
			{
				Client*	client = new Client(*it);
				sender->addToOutputBuffer(RPL_WHOWASUSER(sender->getNickname(), client));
				sender->addToOutputBuffer(RPL_WHOISSERVER(sender->getNickname(), client->getNickname()));
				sender->addToOutputBuffer(RPL_WHOISACTUALLY(sender->getNickname(), client));
				delete client;
				i++;
			}
		}
		if (i == 0)
			sender->addToOutputBuffer(ERR_WASNOSUCHNICK(sender->getNickname(), cmd[1]));
		sender->addToOutputBuffer(RPL_ENDOFWHOWAS(sender->getNickname(), cmd[1]));
	}
}
