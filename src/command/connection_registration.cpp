#include "ircserv.hpp"

void	pass(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (sender->isRegistered() == true)
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname(), cmd[0]));
	else if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (cmd[1].compare(serv->getPassword()))
	{
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname(), cmd[0]));
		sender->setPassword(false);
		// déconnecter le client
	}
	else
		sender->setPassword(true);
}

void	nick(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	if (cmd.size() < 2)
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname(), cmd[0]));
	else
	{
		if (cmd[1].size() > NICKLEN)
			cmd[1] = cmd[1].substr(0, NICKLEN);
		if (cmd[1].find_first_not_of(NICKNAME_CHARSET) != std::string::npos)
			sender->addToOutputBuffer(ERR_ERRONEUSNICKNAME(sender->getNickname(), cmd[0], cmd[1]));
		else if (serv->getAllClients().find(cmd[1]) != serv->getAllClients().end())
			sender->addToOutputBuffer(ERR_NICKNAMEINUSE(sender->getNickname(), cmd[0], cmd[1]));
		else
		{
			if (sender->isRegistered() == true)
			{
				std::string		reply = sender->getPrefix() + " " + cmd[0] + " " + cmd[1];
				sender->addToOutputBuffer(reply);
				sender->sendToAllChannels(reply);
				serv->getAllClients().erase(sender->getNickname());
				serv->getAllClients().insert(std::make_pair(cmd[1], sender));
			}
			sender->setNickname(cmd[1]);
		}
	}
}

void	user(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)serv;
	if (sender->isRegistered() == true)
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname(), cmd[0]));
	else if (cmd.size() < 5)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else
	{
		if (cmd[1].size() > USERLEN)
			cmd[1] = cmd[1].substr(0, USERLEN);
		if (cmd[4].size() > REALNAMELEN)
			cmd[4] = cmd[4].substr(0, REALNAMELEN);
		sender->setUser("~" + cmd[1]);
		sender->setRealName(cmd[4]);
	}
}

void	oper(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)serv;
	if (cmd.size() < 3)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), cmd[0]));
	else if (cmd[1].compare(OPERATOR_USER))
	{
		sender->removeMods("o");
		sender->addToOutputBuffer(ERR_NOOPERHOST(sender->getNickname(), cmd[0]));
	}
	else if (cmd[2].compare(OPERATOR_PASSWORD))
	{
		sender->removeMods("o");
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname(), cmd[0]));
	}
	else
	{
		sender->addMods("o");
		sender->addToOutputBuffer(RPL_YOUREOPER(sender->getNickname()));
	}
}

void	quit(std::vector<std::string> cmd, Client* sender, Server* serv) // mberne
{
	(void)cmd; (void)sender; (void)serv;
}

// pas une commande, mais à envoyer une fois que le client est correctement connecté (pass, nick, user)
void	sendWelcome(Client* sender, Server* serv)
{
	std::string name = sender->getNickname();

	sender->addToOutputBuffer(RPL_WELCOME(name));
	sender->addToOutputBuffer(RPL_YOURHOST(name));
	sender->addToOutputBuffer(RPL_CREATED(name));
	sender->addToOutputBuffer(RPL_MYINFO(name));
	sender->addToOutputBuffer(RPL_ISUPPORT(name));
	sender->addToOutputBuffer(RPL_LUSERCLIENT(name, serv));
	sender->addToOutputBuffer(RPL_LUSEROP(name, serv));
	sender->addToOutputBuffer(RPL_LUSERUNKNOWN(name, serv));
	sender->addToOutputBuffer(RPL_LUSERCHANNELS(name, serv));
	sender->addToOutputBuffer(RPL_LUSERME(name, serv));
	sender->addToOutputBuffer(RPL_MOTDSTART(name));
	sender->addToOutputBuffer(RPL_MOTD(name));
	sender->addToOutputBuffer(RPL_ENDOFMOTD(name));
	sender->addToOutputBuffer(RPL_UMODEIS(name, sender));
}
