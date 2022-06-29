#include "ircserv.hpp"

void	irc_privmsg(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	std::vector<std::string>	target;

	if (cmd.size() > 2)
	{
		parseArg(cmd[1], target);
		for (size_t i = 0; i < target.size(); i++)
		{
			if (target[i][0] != '#')
			{
				if (serv->getClient(target[i]))
				{
					serv->getClient(target[i])->addToOutputBuffer(sender->getPrefix() + " PRIVMSG " + serv->getClient(target[i])->getNickname() + " :" + cmd[2]);
					serv->addLog("from: " + sender->getPrefix() + " to: " + target[i] + "\n" + cmd[2], LOG_MESSAGE);
				}
				else
					sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), target[i]));
			}
			else
			{
				if (serv->getChannel(target[i]) && ableToTalk(sender, serv->getChannel(target[i])))
				{
					serv->getChannel(target[i])->sendToClients(sender->getPrefix() + " PRIVMSG " + serv->getChannel(target[i])->getName() + " :" + cmd[2], sender);
					serv->addLog("from: " + sender->getPrefix() + " to: " + target[i] + "\n" + cmd[2], LOG_MESSAGE);
				}
				else
					sender->addToOutputBuffer(ERR_CANNOTSENDTOCHAN(sender->getNickname(), target[i]));
			}
		}
	}
	else if (cmd.size() == 2)
		sender->addToOutputBuffer(ERR_NOTEXTTOSEND(sender->getNickname()));
	else if (cmd.size() == 1)
		sender->addToOutputBuffer(ERR_NORECIPIENT(sender->getNickname(), cmd[0]));
}

void	irc_notice(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	std::vector<std::string>	target;

	std::cout << "test1\n";
	if (cmd.size() > 2)
	{
		std::cout << "test2\n";
		parseArg(cmd[1], target);
		for (size_t i = 0; i < target.size(); i++)
		{
			std::cout << "test3\n";
			if (!sender)
			{
				std::cout << "test4\n";
				serv->addLog("from: " + SERV_NAME + " to: " + target[i] + "\n" + cmd[2], LOG_MESSAGE);
				serv->getClient(target[i])->addToOutputBuffer(SERV_NAME + " NOTICE " + serv->getClient(target[i])->getNickname() + " :" + cmd[2]);
				std::cout << "test5\n";
			}
			else if (target[i][0] != '#' && serv->getClient(target[i]))
			{
				serv->addLog("from: " + sender->getPrefix() + " to: " + target[i] + "\n" + cmd[2], LOG_MESSAGE);
				serv->getClient(target[i])->addToOutputBuffer(sender->getPrefix() + " NOTICE " + serv->getClient(target[i])->getNickname() + " :" + cmd[2]);
			}
			else if (target[i][0] == '#' && serv->getChannel(target[i]) && ableToTalk(sender, serv->getChannel(target[i])))
			{
				serv->addLog("from: " + sender->getPrefix() + " to: " + target[i] + "\n" + cmd[2], LOG_MESSAGE);
				serv->getChannel(target[i])->sendToClients(sender->getPrefix() + " NOTICE " + serv->getChannel(target[i])->getName() + " :" + cmd[2], sender);
			}
		}
	}
}
