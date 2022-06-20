#include "ircserv.hpp"

// pas commande mais appelé pour répondre à certaines commandes
void	irc_error(Client* sender, std::string reason)
{
	sender->addToOutputBuffer("Error :");
	sender->addToOutputBuffer(reason);	
}

void	irc_kill(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	if (cmd.size() < 3)
		sender->addToOutputBuffer(ERR_NEEDMOREPARAMS(sender->getNickname(), "KILL"));
	else if (!sender->hasMod(CLIENT_FLAG_O))
		sender->addToOutputBuffer(ERR_NOPRIVILEGES(sender->getNickname(), "KILL"));
	else
	{
		Client *client = serv->getClient(cmd[1]);
		if (!client)
			sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), "KILL", cmd[1]));
		else // + deco le gars
			irc_error(sender, "Closing Link: " + std::string(SERV_NAME) + " (Killed (" + sender->getNickname() + " (" + cmd[2] + ")))");
	}
}

// pas commande mais à appeler à chaque tour de boucle de réponse ?
void	irc_ping(Client* client)
{
	time_t	differenceTime = difftime(client->getLastCmdTime(), time(NULL));

	if (differenceTime > MAX_TIME_AFK)
	{
		if (!client->getIsPing())
		{
			client->addToOutputBuffer("PING :");
			client->addToOutputBuffer(SERV_NAME);
			client->setIsPing(1);
		}
		// if (differenceTime > MAX_TIME_AFK + MAX_PING_TIME)
		// 	// couper la connexion avec quit
	}

}

void	irc_pong(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)serv;
	sender->setIsPing(0);
}
