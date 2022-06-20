#include "ircserv.hpp"

void	irc_pass(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (sender->isRegistered() == true)
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname()));
	else if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (cmd[1].compare(serv->getPassword()))
	{
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname()));
		sender->setPassword(false);
		// déconnecter le client error() + removeClient()
	}
	else
		sender->setPassword(true);
}

void	irc_nick(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname()));
	else
	{
		if (cmd[1].size() > NICK_LEN)
			cmd[1] = cmd[1].substr(0, NICK_LEN);
		if (cmd[1].find_first_not_of(ASCII_CHARSET) != std::string::npos)
			sender->addToOutputBuffer(ERR_ERRONEUSNICKNAME(sender->getNickname(), cmd[1]));
		else if (serv->getAllClients().find(cmd[1]) != serv->getAllClients().end())
			sender->addToOutputBuffer(ERR_NICKNAMEINUSE(sender->getNickname(), cmd[1]));
		else
		{
			if (sender->isRegistered() == true)
			{
				std::string		reply = sender->getPrefix() + " " + cmd[0] + " " + cmd[1];
				sender->addToOutputBuffer(reply);
				sender->sendToAllChannels(reply);
				serv->getAllClients().erase(sender->getNickname());
				serv->getAllClients().insert(std::make_pair(cmd[1], sender));
				serv->addOldNickname(sender->getNickname(), sender);
			}
			sender->setNickname(cmd[1]);
		}
	}
}

void	irc_user(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (sender->isRegistered() == true)
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname()));
	else if (cmd.size() < 5)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else
	{
		if (cmd[1].size() > USER_LEN)
			cmd[1] = cmd[1].substr(0, USER_LEN);
		if (cmd[4].size() > REALNAME_LEN)
			cmd[4] = cmd[4].substr(0, REALNAME_LEN);
		sender->setUser("~" + cmd[1]);
		sender->setRealName(cmd[4]);
	}
}

void	irc_oper(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() < 3)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (cmd[1].compare(OPERATOR_USER))
	{
		sender->removeMods("o");
		sender->addToOutputBuffer(ERR_NOOPERHOST(sender->getNickname()));
	}
	else if (cmd[2].compare(OPERATOR_PASSWORD))
	{
		sender->removeMods("o");
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname()));
	}
	else
	{
		sender->addMods("o");
		sender->addToOutputBuffer(RPL_YOUREOPER(sender->getNickname()));
	}
}

void	irc_quit(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	std::string	leaveMsg;

	if (cmd.size() == 1)
		leaveMsg = sender->getPrefix() + " " + cmd[0] + " :Client Quit";
	else
		leaveMsg = sender->getPrefix() + " " + cmd[0] + " :" + cmd[1];
	sender->addToOutputBuffer(leaveMsg);
	irc_error(sender, "Closing Link: " + sender->getHost());
	send(sender->getSock(), sender->getOutputBuffer(), strlen(sender->getOutputBuffer()), 0);
	sender->sendToAllChannels(leaveMsg);
	// removeClient(sender);
}
