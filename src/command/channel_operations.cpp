#include "ircserv.hpp"

void	irc_join(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (!cmd[1].compare("0"))
		sender->leaveAllChannels(serv);
	else
	{
		std::vector<std::string>	channels, password;

		parseArg(cmd[1], channels);
		if (cmd.size() > 2)
			parseArg(cmd[2], password);
		while (password.size() < channels.size())
			password.push_back("");

		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].size() > CHANNEL_LEN)
				channels[i] = channels[i].substr(0, CHANNEL_LEN);
			Channel* current = serv->getChannel(channels[i]);

			if (channels[i][0] != '#' || channels[i].find_first_of(" ,") != std::string::npos || channels[i].find(7) != std::string::npos)
				sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), channels[i]));
			else if (sender->getNumberOfChannels() >= CLIENT_CHANNELS_LIMIT)
			{
				sender->addToOutputBuffer(ERR_TOOMANYCHANNELS(sender->getNickname(), channels[i]));
				return;	
			}
			else if (current && !current->getPassword().empty() && current->getPassword().compare(password[i]))
				sender->addToOutputBuffer(ERR_BADCHANNELKEY(sender->getNickname(), channels[i]));
			else if (current && current->isBanned(sender))
				sender->addToOutputBuffer(ERR_BANNEDFROMCHAN(sender->getNickname(), channels[i]));
			else if (current && current->hasModes(CHANNEL_FLAG_I) && !current->isInvited(sender))
				sender->addToOutputBuffer(ERR_INVITEONLYCHAN(sender->getNickname(), channels[i]));
			else if (current && current->clientCount() >= current->getUserLimit())
				sender->addToOutputBuffer(ERR_CHANNELISFULL(sender->getNickname(), channels[i]));
			else
			{
				if (!current)
				{
					if (serv->getAllChannels().size() >= CHANNEL_LIMIT)
					{
						irc_notice(vectorizator("NOTICE", sender->getNickname(), "Couldn't create channel " + channels[i] + ", server channel limit reached"), NULL, serv);
						return;
					}
					else
						current = serv->addChannel(channels[i], sender);
				}
				else if (current->getClient(sender->getNickname()) != NULL)
					return;

				sender->joinChannel(current);
				current->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + channels[i], NULL);
				if (current->hasModes(CHANNEL_FLAG_T) == true)
					sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(), current));
				irc_names(vectorizator("NAMES", current->getName(), ""), sender, serv);
			}
		}
	}
}

void	irc_part(std::vector<std::string> cmd, Client* sender, Server* serv)
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
				sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), channels[i]));
			else if (current->getClient(sender->getNickname()) == NULL)
				sender->addToOutputBuffer(ERR_NOTONCHANNEL(sender->getNickname(), channels[i]));
			else
			{
				current->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + channels[i] + (cmd.size() > 2 ? " " + cmd[2] : "" ), NULL);
				sender->leaveChannel(current, serv);
			}
		}
	}
}

void	userMode(std::vector<std::string> cmd, Client* sender)
{
	std::string		modeString = cmd[2];
	std::string		validModes;

	if (modeString.find_first_not_of("+-" + USER_MODES) != std::string::npos)
		sender->addToOutputBuffer(ERR_UMODEUNKNOWNFLAG(sender->getNickname()));
	for (size_t i = 0; i < USER_MODES.size(); i++)
	{
		size_t	flagPos = modeString.find_last_of(USER_MODES[i]);
		size_t	signPos = modeString.find_last_of("+-", flagPos);
		char	sign	= (signPos == std::string::npos ? '+' : modeString[signPos]);
		if (flagPos != std::string::npos)
		{
			validModes.push_back(sign);
			validModes += USER_MODES[i];
		}
	}
	if (!validModes.empty())
	{
		std::string reply = sender->setModes(validModes);
		if (!reply.empty())
			sender->addToOutputBuffer(sender->getPrefix() + " " + cmd[0] + " " + sender->getNickname() + " :" + reply);
	}
}

void	channelMode(std::vector<std::string> cmd, Client* sender, Server* serv, Channel* channel)
{
	std::string					modeString = cmd[2];
	std::string					validModes;
	std::map<char, std::string>	modesArgs;
	size_t						argIndex = 3;

	for (size_t i = 0; i < CHANNEL_MODES.size(); i++)
	{
		size_t					flagPos = modeString.find_last_of(CHANNEL_MODES[i]);
		std::string::iterator	end = modeString.begin() + flagPos;

		if (flagPos != std::string::npos)
			modeString.erase(remove(modeString.begin(), end, CHANNEL_MODES[i]), end);
	}

	for (size_t i = 0; i < modeString.size(); i++)
	{
		size_t	signPos = modeString.find_last_of("+-", i);
		char	sign	= (signPos == std::string::npos ? '+' : modeString[signPos]);

		if (std::string("psitnm").find(modeString[i]) != std::string::npos)
		{
			validModes.push_back(sign);
			validModes.push_back(modeString[i]);
		}
		else if (std::string("ov").find(modeString[i]) != std::string::npos)
		{
			if (cmd.size() == argIndex)
			{
				sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
				return;
			}
			if (serv->getClient(cmd[argIndex]) == NULL)
				sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[argIndex++]));
			else if (channel->getClient(cmd[argIndex]) == NULL)
				sender->addToOutputBuffer(ERR_USERNOTINCHANNEL(sender->getNickname(), cmd[argIndex++], cmd[1]));
			else
			{
				validModes.push_back(sign);
				validModes.push_back(modeString[i]);
				modesArgs.insert(std::make_pair(modeString[i], cmd[argIndex++]));
			}
		}
		else if (std::string("l").find(modeString[i]) != std::string::npos)
		{
			if (sign == '+' && cmd.size() == argIndex)
			{
				sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
				return;
			}
			else if (sign == '-')
			{
				validModes.push_back(sign);
				validModes.push_back(modeString[i]);
			}
			else
			{
				size_t limit = (std::isdigit(cmd[argIndex][0]) ? std::stoi(cmd[argIndex]) : 0);
				if (limit)
				{
					validModes.push_back(sign);
					validModes.push_back(modeString[i]);
					modesArgs.insert(std::make_pair(modeString[i], cmd[argIndex++]));
				}
			}
		}
		else if (std::string("b").find(modeString[i]) != std::string::npos)
		{
			if (cmd.size() == argIndex)
			{
				for (std::map<std::string, BanMask>::iterator it = channel->getBanList().begin(); it != channel->getBanList().end(); it++)
					sender->addToOutputBuffer(RPL_BANLIST(sender->getNickname(), cmd[1], it->second.getBanMask()));
				sender->addToOutputBuffer(RPL_ENDOFBANLIST(sender->getNickname(), cmd[1]));
			}
			else
			{
				validModes.push_back(sign);
				validModes.push_back(modeString[i]);
				modesArgs.insert(std::make_pair(modeString[i], cmd[argIndex++]));
			}
		}
		else if (std::string("k").find(modeString[i]) != std::string::npos)
		{
			if (cmd.size() == argIndex)
			{
				sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
				return;
			}
			else if (sign == '+' && !cmd[argIndex].compare(channel->getPassword()))
			{
				sender->addToOutputBuffer(ERR_KEYSET(sender->getNickname(), cmd[0]));
				argIndex++;
			}
			else
			{
				validModes.push_back(sign);
				validModes.push_back(modeString[i]);
				modesArgs.insert(std::make_pair(modeString[i], cmd[argIndex++]));
			}
		}
	}
	if (!validModes.empty())
	{
		std::string reply = channel->setModes(validModes, modesArgs);
		if (!reply.empty())
			channel->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + channel->getName() + " " + reply, NULL);
	}
}

void	irc_mode(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (cmd[1].size() > 0 && cmd[1][0] != '#') // USER MODE
	{
		if (serv->getClient(cmd[1]) == NULL)
			sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[1]));
		else if (sender->getNickname().compare(cmd[1]) != 0)
			sender->addToOutputBuffer(ERR_USERSDONTMATCH(sender->getNickname()));
		else if (cmd.size() < 3)
			sender->addToOutputBuffer(RPL_UMODEIS(sender->getNickname(), sender));
		else
			userMode(cmd, sender);
	}
	else // CHANNEL MODE
	{
		Channel* channel = serv->getChannel(cmd[1]);

		if (channel == NULL)
			sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[1]));
		else if (cmd.size() < 3)
			sender->addToOutputBuffer(RPL_CHANNELMODEIS(sender->getNickname(), channel));
		else if (channel->isOperator(sender) == false)
			sender->addToOutputBuffer(ERR_CHANOPRIVSNEEDED(sender->getNickname(), channel->getName()));
		else if (cmd[2].find_first_not_of("+-" + CHANNEL_MODES) != std::string::npos)
			sender->addToOutputBuffer(ERR_UNKNOWNMODE(sender->getNickname(), cmd[2][cmd[2].find_first_not_of("+-" + CHANNEL_MODES)]));
		else
			channelMode(cmd, sender, serv, channel);
	}
}

void	irc_topic(std::vector<std::string> cmd, Client* sender, Server* serv)
{

	if (cmd.size() < 2)
	{
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
		return;
	}
	Channel* channel = serv->getChannel(cmd[1]);

	if (channel == NULL)
		sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[1]));
	else if (sender->getChannel(cmd[1]) == NULL)
		sender->addToOutputBuffer(ERR_NOTONCHANNEL(sender->getNickname(), cmd[1]));
	else if (cmd.size() == 2)
	{
		if (channel->getTopic().empty())
			sender->addToOutputBuffer(RPL_NOTOPIC(sender->getNickname(), channel));
		else
		{
			sender->addToOutputBuffer(RPL_TOPIC(sender->getNickname(),channel));
			sender->addToOutputBuffer(RPL_TOPICWHOTIME(sender->getNickname(), channel));
		}
	}
	else if (channel->hasModes(CHANNEL_FLAG_T) && !channel->isOperator(sender))
		sender->addToOutputBuffer(ERR_CHANOPRIVSNEEDED(sender->getNickname(), cmd[1]));
	else
	{
		if (cmd[2].size() > TOPIC_LEN)
			cmd[2] = cmd[2].substr(0, TOPIC_LEN);
		channel->setTopic(cmd[2]);
		channel->setTopicTime();
		channel->setTopicWriter(sender->getNickname());
		channel->sendToClients(RPL_TOPIC(sender->getNickname(),channel), NULL);
	}
}

void	irc_names(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2)
	{
		for (std::map<std::string, Channel*>::iterator it = serv->getAllChannels().begin(); it != serv->getAllChannels().end(); it++)
		{
			if ((!it->second->hasModes(CHANNEL_FLAG_S) && !it->second->hasModes(CHANNEL_FLAG_P)) || it->second->getClient(sender->getNickname()) != NULL)
			{
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), it->second->getName(), it->second->showClientsList()));
				sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), it->second->getName()));
			}
		}
		sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), "*", serv->showLonelyClientsList()));
		sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), "*"));
	}
	else
	{
		std::vector<std::string>	channels;

		parseArg(cmd[1], channels);
		for (size_t i = 0; i < channels.size(); i++)
		{
			Channel* current = serv->getChannel(channels[i]);

			if (current != NULL && ((!current->hasModes(CHANNEL_FLAG_S) && !current->hasModes(CHANNEL_FLAG_P)) || current->getClient(sender->getNickname()) != NULL))
				sender->addToOutputBuffer(RPL_NAMREPLY(sender->getNickname(), current->getName(), current->showClientsList()));
			sender->addToOutputBuffer(RPL_ENDOFNAMES(sender->getNickname(), channels[i]));
		}
	}
}

void	irc_list(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	sender->addToOutputBuffer(RPL_LISTSTART(sender->getNickname()));
	if (cmd.size() < 2)
	{
		for (std::map<std::string, Channel*>::iterator it = serv->getAllChannels().begin(); it != serv->getAllChannels().end(); it++)
			if ((!it->second->hasModes(CHANNEL_FLAG_S) && !it->second->hasModes(CHANNEL_FLAG_P)) || it->second->getClient(sender->getNickname()) != NULL)
				sender->addToOutputBuffer(RPL_LIST(sender->getNickname(), it->second));
	}
	else
	{
		std::vector<std::string>	channels;

		parseArg(cmd[1], channels);
		for (std::vector<std::string>::iterator it = channels.begin(); it < channels.end(); it++)
		{
			Channel* current = serv->getChannel(*it);
			if (current != NULL && ((!current->hasModes(CHANNEL_FLAG_S) && !current->hasModes(CHANNEL_FLAG_P)) || current->getClient(sender->getNickname()) != NULL))
				sender->addToOutputBuffer(RPL_LIST(sender->getNickname(), current));
		}
	}
	sender->addToOutputBuffer(RPL_LISTEND(sender->getNickname()));
}

void	irc_invite(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() == 1)
	{
		for (std::map<std::string, Channel*>::iterator it = serv->getAllChannels().begin(); it != serv->getAllChannels().end(); it++)
			if (it->second->isInvited(sender))
				sender->addToOutputBuffer(RPL_INVITELIST(sender->getNickname(), it->second));
		sender->addToOutputBuffer(RPL_ENDOFINVITELIST(sender->getNickname()));
	}
	else if (cmd.size() == 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else
	{
		Client*		client = serv->getClient(cmd[1]);
		Channel*	channel = serv->getChannel(cmd[2]);

		if (!client)
			sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[1]));
		else if (!channel)
			sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[2]));
		else if (!channel->getClient(sender->getNickname()))
			sender->addToOutputBuffer(ERR_NOTONCHANNEL(sender->getNickname(), cmd[2]));
		else if (channel->getClient(client->getNickname()))
			sender->addToOutputBuffer(ERR_USERONCHANNEL(sender->getNickname(), cmd[1], cmd[2]));
		else if (channel->hasModes(CHANNEL_FLAG_I) && !channel->isOperator(sender))
			sender->addToOutputBuffer(ERR_CHANOPRIVSNEEDED(sender->getNickname(), cmd[2]));
		else
		{
			sender->addToOutputBuffer(RPL_INVITING(sender->getNickname(), cmd[1], cmd[2]));
			client->addToOutputBuffer(sender->getPrefix() + " " + cmd[0] + " " + cmd[1] + " :" + cmd[2]);
			channel->addInvitedClient(client);
		}
	}
}

void	irc_kick(std::vector<std::string> cmd, Client* sender, Server* serv)
{

	if (cmd.size() < 3)
	{
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
		return;
	}
	Channel*	channel = serv->getChannel(cmd[1]);
	Client*		client = serv->getClient(cmd[2]);
	
	if (channel == NULL)
		sender->addToOutputBuffer(ERR_NOSUCHCHANNEL(sender->getNickname(), cmd[1]));
	else if (channel->getClient(sender->getNickname()) == NULL)
		sender->addToOutputBuffer(ERR_NOTONCHANNEL(sender->getNickname(), cmd[1]));
	else if (channel->isOperator(sender) == false)
		sender->addToOutputBuffer(ERR_CHANOPRIVSNEEDED(sender->getNickname(), cmd[1]));
	else if (client == NULL)
		sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[2]));
	else if (channel->getClient(cmd[2]) == NULL)
		sender->addToOutputBuffer(ERR_USERNOTINCHANNEL(sender->getNickname(), cmd[2], cmd[1]));
	else
	{
		channel->sendToClients(sender->getPrefix() + " " + cmd[0] + " " + cmd[1] + " " + cmd[2] + " :" + (cmd.size() > 3 ? cmd[3] : "Kicked by operator." ), NULL);
		client->leaveChannel(channel, serv);
	}
}
