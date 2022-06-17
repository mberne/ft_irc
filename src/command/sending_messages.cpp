#include "ircserv.hpp"

void	irc_privmsg(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
	// ERR_NOSUCHNICK ERR_NOSUCHSERVER ERR_CANNOTSENDTOCHAN ERR_TOOMANYTARGETS ERR_NORECIPIENT ERR_NOTEXTTOSEND ERR_NOTOPLEVEL ERR_WILDTOPLEVEL
}

void	irc_notice(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	std::vector<std::string>	target;

	if (cmd.size() > 2)
	{
		parseArg(cmd[1], target);
		for (size_t i = 0; i < target.size(); i++)
		{
			if (serv->getChannel(target[i]))
				serv->getChannel(target[i])->sendToClients(sender->getPrefix() + " NOTICE " + serv->getClient(target[i])->getNickname() + " " + cmd[2]);
			else if (serv->getClient(target[i]))
				sender->addToOutputBuffer(sender->getPrefix() + " NOTICE " + serv->getClient(target[i])->getNickname() + " " + cmd[2]);
		}
	}
}
