#include "ircserv.hpp"

void	irc_join(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (!cmd[1].compare("0"))
		sender->leaveAllChannels(serv);
	else
	{
		std::vector<std::string>	channels, password;

		parseArg(cmd[1], channels);
		parseArg(cmd[2], password);
		while (password.size() < channels.size())
			password.push_back("");
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].size() > CHANNELLEN)
				channels[i] = channels[i].substr(0, CHANNELLEN);
			Channel* current = serv->getChannel(channels[i]);

			if (channels[i][0] != '#')
				sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[0], channels[i]));
			else if (sender->getNumberOfChannels() >= CHANLIMIT)
			{
				sender->addToOutputBuffer(ERR_TOOMANYCHANNELS(sender->getNickname(), cmd[0], channels[i]));
				return;	
			}
			else if (!current)
			{
				current = serv->newChannel(channels[i], sender);
				sender->joinChannel(current);
				sender->addToOutputBuffer(sender->getPrefix() + " " + cmd[0] + " " + channels[i]);
				if (current->hasTopic() == true)
					sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(), current));
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), current));
				sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), channels[i]));
			}
			else if (!current->getPassword().empty())
			{
				if (current->getPassword().compare(password[i]))
					sender->addToOutputBuffer(ERR_BADCHANNELKEY(sender->getNickname(), cmd[0], channels[i]));
			}
			else if (current->isBanned(sender))
				sender->addToOutputBuffer(ERR_BANNEDFROMCHAN(sender->getNickname(), cmd[0], channels[i]));
			else if (!current->isInvited(sender))
				sender->addToOutputBuffer(ERR_INVITEONLYCHAN(sender->getNickname(), cmd[0], channels[i]));
			else if (current->clientCount() >= current->getUserLimit())
				sender->addToOutputBuffer(ERR_CHANNELISFULL(sender->getNickname(), cmd[0], channels[i]));
			// ERR_BADCHANMASK
			else
			{
				sender->joinChannel(current);
				current->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + channels[i]);
				if (current->hasTopic() == true)
					sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(), current));
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), current));
				sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), channels[i]));
			}
		}
	}
}

void	irc_part(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else
	{
		std::vector<std::string>	channels;

		parseArg(cmd[1], channels);
		for (size_t i = 0; i < channels.size(); i++)
		{
			Channel* current = serv->getChannel(channels[i]);

			if (current == NULL)
				sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[0], channels[i]));
			else if (current->getClient(sender->getNickname()) == NULL)
				sender->addToOutputBuffer(ERR_NOTONCHANNEL(sender->getNickname(), cmd[0], channels[i]));
			else
			{
				current->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + channels[i] + (cmd.size() > 2 ? " " + cmd[2] : "" ));
				sender->leaveChannel(current);
				if (current->clientCount() == 0)
					serv->removeChannel(current);
			}
		}
	}
}

void	irc_mode(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	irc_topic(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	irc_names(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() == 1)
	{
		for (std::map<std::string, Channel*>::iterator itChan = serv->getAllChannels().begin(); itChan != serv->getAllChannels().end(); itChan++)
			for (std::map<std::string, Client*>::iterator itCli = itChan->second->getAllClients().begin(); itCli != itChan->second->getAllClients().end(); itCli++)
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), itChan->second));
		// afficher ceux sans canal
	}
	else // doublons à gérer ?
		for (size_t i = 1; i != cmd.size(); i++)
			for (std::map<std::string, Channel*>::iterator itChan = serv->getAllChannels().begin(); itChan != serv->getAllChannels().end(); itChan++)
			{
				if (!itChan->second->getName().compare(cmd[i]))
				{
					for (std::map<std::string, Client*>::iterator itCli = itChan->second->getAllClients().begin(); itCli != itChan->second->getAllClients().end(); itCli++)
						sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), itChan->second));
				}
			}
	sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), cmd[1]));
}

void	irc_list(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	// ERR_NOSUCHSERVER ??
	// doublons à gérer... oskour
	bool	arg = 0;
	if (cmd.size() > 1)
		arg = 1;
	sender->addToOutputBuffer(RPL_LISTSTART(sender->getNickname()));
	if (!arg)
		for (std::map<std::string, Channel*>::iterator it = serv->getAllChannels().begin(); it != serv->getAllChannels().end(); it++)
			sender->addToOutputBuffer(RPL_LIST(sender->getNickname(), it->second));
	else
		for (size_t i = 1; i != cmd.size(); i++)
			for (std::map<std::string, Channel*>::iterator itChan = serv->getAllChannels().begin(); itChan != serv->getAllChannels().end(); itChan++)
			{
				if (!itChan->second->getName().compare(cmd[i]))
					sender->addToOutputBuffer(RPL_LIST(sender->getNickname(), itChan->second));
			}
	sender->addToOutputBuffer(RPL_LISTEND(sender->getNickname()));
}

void	irc_kick(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}
