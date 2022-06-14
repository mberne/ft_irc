#include "ircserv.hpp"

void	pass(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 3)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), "PASS"));
	else if (sender->isRegistered())
		sender->addToOutputBuffer(ERR_ALREADYREGISTRED(sender->getNickname(), cmd[0]));
	else if (!cmd[1].compare(serv->getPassword()))
	{
		sender->setPassword(0);
		sender->addToOutputBuffer(ERR_PASSWDMISMATCH(sender->getNickname(), cmd[0]));
	}
	else
		sender->setPassword(true);
}

void	nick(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	if (cmd.size() < 3)
	{
		sender->addToOutputBuffer(ERR_NONICKNAMEGIVEN(sender->getNickname(), cmd[0]));
		return;
	}
	if (cmd[1].size() > MAX_NICKNAME_LENGTH)
		cmd[1] = cmd[1].substr(0, MAX_NICKNAME_LENGTH);
	if (cmd[1].find_first_of(SUPPORTED_NICKNAME_CHAR) != std::string::npos)
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
		}
		sender->setNickname(cmd[1]);
	}
}

void	user(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	oper(std::vector<std::string> cmd, Client* sender, Server* serv) // pthomas
{
	(void)cmd; (void)sender; (void)serv;
}

void	quit(std::vector<std::string> cmd, Client* sender, Server* serv) // mberne
{
	(void)cmd; (void)sender; (void)serv;
}

// pas une commande, mais à envoyer une fois que le client est correctement connecté (pass, nick, user)
void	registrationDone(std::vector<std::string> cmd, Client* sender, Server* serv)
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

// ERR_NONICKNAMEGIVEN
// Renvoyé quand un paramètre pseudonyme attendu pour une commande n'est pas fourni.
// ERR_ERRONEUSNICKNAME
// Renvoyé après la réception d'un message NICK qui contient des caractères qui ne font pas partie du jeu autorisé. Voir les sections 1 et 2.2 pour les détails des pseudonymes valides.
