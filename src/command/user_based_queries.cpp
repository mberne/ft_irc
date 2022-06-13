#include "ircserv.hpp"

void	who(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
	// //RPL_WHOREPLY FOR CHANNEL TOO...
	// if (cmd.size() < 2)
	// 	sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "WHO", cmd[1])); // ?
	// else if (serv->getClient(cmd[1]))
	// 	sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), serv->getClient(cmd[1])));
	// // else if (serv->getChannel(cmd[1]))
	// // 	sender->addToOutputBuffer(RPL_WHOREPLY(sender->getNickname(), serv->getChannel(cmd[1])));
	// sender->addToOutputBuffer(RPL_ENDOFWHO(sender->getNickname(), cmd[1]));
}

void	whois(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
	// // ERR_NONICKNAMEGIVEN gere ou ?
	// if (cmd.size() < 2 && !cmd[1].compare(SERV_NAME))
	// 	sender->addToOutputBuffer(ERR_NOSUCHSERVER(sender->getNickname(), "WHOIS", cmd[1]));
	// else
	// {
	// 	if (cmd.size() < 3)
	// 	{
	// 		if (serv->getClient(cmd[1]))
	// 		{
	// 			sender->addToOutputBuffer(RPL_WHOISUSER(sender->getNickname(), serv->getClient(cmd[1])));
	// 			sender->addToOutputBuffer(RPL_WHOISSERVER(sender->getNickname(), cmd[1]));
	// 			sender->addToOutputBuffer(RPL_WHOISOPERATOR(sender->getNickname(), cmd[1]));
	// 			sender->addToOutputBuffer(RPL_WHOISCHANNELS(sender->getNickname(), cmd[1], serv->getClient(cmd[1])));
	// 			sender->addToOutputBuffer(RPL_ENDOFWHOIS(sender->getNickname(), cmd[1]));
	// 		}
	// 		else
	// 			ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[1]);
	// 	}
	// 	else
	// 	{
	// 		if (serv->getClient(cmd[2]))
	// 		{
	// 			sender->addToOutputBuffer(RPL_WHOISUSER(sender->getNickname(), serv->getClient(cmd[2])));
	// 			sender->addToOutputBuffer(RPL_WHOISSERVER(sender->getNickname(), cmd[2]));
	// 			sender->addToOutputBuffer(RPL_WHOISOPERATOR(sender->getNickname(), cmd[2]));
	// 			sender->addToOutputBuffer(RPL_WHOISCHANNELS(sender->getNickname(), cmd[2], serv->getClient(cmd[2])));
	// 			sender->addToOutputBuffer(RPL_ENDOFWHOIS(sender->getNickname(), cmd[2]));
	// 		}
	// 		else
	// 			sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[2]));
	// 	}
	// }
}

void	whowas(std::vector<std::string> cmd, Client* sender, Server* serv)
{
	(void)cmd; (void)sender; (void)serv;
	// ERR_NONICKNAMEGIVEN gere ou ?
	// int count;
	// int i;

	// if (cmd.size() < 2)
	// {
	// 	count = std::stoi(cmd[2]);
	// 	if (count < 1)
	// 		count = INT32_MAX;
	// }
	// if (getOldNickmame(cmd[1]))
	// {
	// 	while (i < count)
	// 	{
	// 		// RPL_WHOWASUSER(sender->getNickname(), ?);
	// 		i++;
	// 	}
	// }
	// else
	// 	sender->addToOutputBuffer(ERR_NOSUCHNICK(sender->getNickname(), cmd[0], cmd[1]));
	// sender->addToOutputBuffer(RPL_ENDOFWHOWAS(sender->getNickname(), cmd[1]));
}
