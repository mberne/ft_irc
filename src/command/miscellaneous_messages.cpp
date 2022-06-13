#include "ircserv.hpp"

// Lors de la connexion à un serveur IRC, on envoie au client le MOTD (s'il est présent) ainsi que le nombre actuel d'utilisateurs et de serveurs (comme pour la commande LUSER).
// Le serveur doit également envoyer un message non équivoque au client, qui stipule son nom, sa version, ainsi que tout autre message d'introduction qui lui semble approprié.
// Après cela, le serveur doit envoyer le pseudo du nouvel utilisateur, et d'autres informations aussi bien fournies par lui-même (commande USER) que découvertes par le serveur (de la part des serveurs DNS et IDENT).
// Le serveur doit envoyer ces informations à la première commande NICK suivie de USER.
// void	motd(std::vector<std::string> cmd, Client* sender, Server* serv) {}

void	version(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "VERSION", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_VERSION(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ISUPPORT(sender->getNickname()));
	}
}

void	stats(std::vector<std::string> cmd, Client* sender, Server* serv) // needtofix
{
	if (cmd.size() < 2 && !cmd[2].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[1]));
	else if (cmd.size() < 3)
	{
		if (cmd.size() < 2)
		{
			sender->addToOutputBuffer(RPL_STATSUPTIME(sender->getNickname(), serv));
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), ""));
		}
		else if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME) && cmd[1].size() == 1)
			sender->addToOutputBuffer(RPL_ENDOFSTATS(sender->getNickname(), cmd[1]));
		else if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME) && cmd[1].size() != 1)
			sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "STATS", cmd[1]));
	}
}

void	time(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "TIME", cmd[1]));
	else
		sender->addToOutputBuffer(RPL_TIME(sender->getNickname(), serv->currentTime()));
}

void	admin(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "ADMIN", cmd[1]));
	else
	{
		sender->addToOutputBuffer(RPL_ADMINME(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC1(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINLOC2(sender->getNickname()));
		sender->addToOutputBuffer(RPL_ADMINEMAIL(sender->getNickname()));
	}
}

void	info(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)serv;
	if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME))
		sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "INFO", cmd[1]));
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

void	lusers(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd;
	sender->addToOutputBuffer(RPL_LUSERCLIENT(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSEROP(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERUNKNOWN(sender->getNickname(), serv));
	sender->addToOutputBuffer(RPL_LUSERCHANNELS(sender->getNickname(), serv));
}
