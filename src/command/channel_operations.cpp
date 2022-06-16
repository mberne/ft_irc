#include "ircserv.hpp"

void	parseJoinCommand(std::vector<std::string> cmd, std::vector<std::string>& cmdChannels, std::vector<std::string>& cmdPasswords)
{
	std::vector<std::string>	newcmd;
	size_t						commaPos = cmd[1].find(',');
	size_t						tmp = 0;

	while (commaPos != std::string::npos)
	{
		cmdChannels.push_back(cmd[1].substr(tmp, commaPos - tmp));
		tmp = commaPos + 1;
		commaPos = cmd[1].find(',', tmp);
	}
	cmdChannels.push_back(cmd[1].substr(tmp, cmd[1].size() - tmp));
	commaPos =cmd[2].find(',');
	tmp = 0;
	while (commaPos != std::string::npos || cmdPasswords.size() < cmdChannels.size())
	{
		if (commaPos == std::string::npos)
			cmdPasswords.push_back("");
		else
		{
			cmdPasswords.push_back(cmd[2].substr(tmp, commaPos - tmp));
			tmp = commaPos + 1;
			commaPos = cmd[2].find(',', tmp);
		}
	}
	cmdPasswords.push_back(cmd[2].substr(tmp, cmd[2].size() - tmp));
}

void	join(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (!cmd[1].compare("0"))
		sender->leaveAllChannels();
	else
	{
		std::vector<std::string>	cmdChannels, cmdPasswords;

		parseJoinCommand(cmd, cmdChannels, cmdPasswords);
		for(size_t i = 0; i < cmdChannels.size(); i++)
		{
			if (cmdChannels[i].size() > CHANNELLEN)
				cmdChannels[i] = cmdChannels[i].substr(0, CHANNELLEN);
			Channel* channel = serv->getChannel(cmdChannels[i]);

			if (cmdChannels[i][0] != '#')
				sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[0], cmdChannels[i]));
			else if (sender->getNumberOfChannels() >= CHANLIMIT)
			{
				sender->addToOutputBuffer(ERR_TOOMANYCHANNELS(sender->getNickname(), cmd[0], cmdChannels[i]));
				return;	
			}
			else if (!channel)
			{
				channel = serv->newChannel(cmdChannels[i], sender);
				sender->joinChannel(channel);
				sender->addToOutputBuffer(sender->getPrefix() + " " + cmd[0] + " " + cmdChannels[i]);
				if (channel->hasTopic() == true)
					sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(), channel));
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), channel));
				sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), channel->getName()));
			}
			else if (!channel->getPassword().empty())
			{
				if (channel->getPassword().compare(cmdPasswords[i]))
					sender->addToOutputBuffer(ERR_BADCHANNELKEY(sender->getNickname(), cmd[0], cmdChannels[i]));
			}
			else if (channel->isBanned(sender))
				sender->addToOutputBuffer(ERR_BANNEDFROMCHAN(sender->getNickname(), cmd[0], cmdChannels[i]));
			else if (!channel->isInvited(sender))
				sender->addToOutputBuffer(ERR_INVITEONLYCHAN(sender->getNickname(), cmd[0], cmdChannels[i]));
			else if (channel->clientCount() >= channel->getUserLimit())
				sender->addToOutputBuffer(ERR_CHANNELISFULL(sender->getNickname(), cmd[0], cmdChannels[i]));
			// ERR_BADCHANMASK
			else
			{
				sender->joinChannel(channel);
				channel->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + cmdChannels[i]);
				if (channel->hasTopic() == true)
					sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(), channel));
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), channel));
				sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), channel->getName()));
			}
		}
	}
}

void	part(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	mode(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	topic(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	names(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
}

void	list(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
}

void	kick(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}
