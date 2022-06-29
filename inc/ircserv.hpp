#ifndef IRCSERV_H
# define IRCSERV_H

/*** ~~ HEADERS ~~ ***/
// Standard libs
# include <cstdlib>
# include <iostream>
# include <string>
# include <csignal>
// Classes
class Server;
class Client;
class Channel;
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

/*** ~~ DEFINES ~~ ***/

// Utils
# define USAGE						std::string("usage: ircserv <port> <password>")
# define PROMPT(num, user)			std::string(":") + SERV_NAME + std::string(num) + user
# define START_LOG					"Server started:\t" + SERV_NAME +\
									"\nVersion:\t\t" + SERV_VERSION +\
									"\nCreation:\t\t" + SERV_CREATION +\
									"\nAdmin:\t\t\t" + SERV_ADMIN +\
									"\nAdmin email:\t" + SERV_ADMIN_EMAIL +\
									"\nInfo:\t\t\t" + SERV_INFO +\
									"\nLocation:\t\t" + SERV_LOC1 +\
									" " + SERV_LOC2
// Server info
# define SERV_NAME					std::string("potatoes.land")
# define SERV_VERSION				std::string("4.2")
# define SERV_CREATION				std::string("Thursday 9 June 2022 at 10:42:00 UTC")
# define SERV_ADMIN					std::string("Pinkie Pie")
# define SERV_ADMIN_EMAIL			std::string("pinkie_pie@rainbow.dash")
# define SERV_INFO					std::string("Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft.")
# define SERV_LOC1					std::string("Charbonniere-Les-Bains, France")
# define SERV_LOC2					std::string("42 Lyon")
// Server specifications
# define CASE_MAPPING				std::string("ascii")
# define USER_MODES					std::string("i")
# define CHANNEL_MODES				std::string("opsitnmlbvk")
# define CHAN_TYPES					std::string("#")
# define NICK_LEN					20
# define USER_LEN					15
# define HOST_LEN					15
# define REALNAME_LEN				50
# define MESSAGE_LEN				512
# define CHANNEL_LEN				50
# define TOPIC_LEN					400
# define CLIENT_LIMIT				500
# define CHANNEL_LIMIT				2000
# define CLIENT_CHANNELS_LIMIT		10
# define CHANNEL_CLIENTS_LIMIT		100
// Other specifications
# define CONNEXION_TIME				120
# define MAX_PASS_ATTEMPT			3
# define TIME_AFK					300
# define PING_TIME					60
# define OLD_CLIENT_LIMIT			400
# define ASCII_CHARSET				std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
# define CRLF						std::string("\r\n")
# define OPERATOR_USER				std::string("p1k13p13")
# define OPERATOR_PASSWORD			std::string("*_*SEGFAULT_11")

/*** ~~ PROTOTYPES ~~ ***/

// Server commands
// Channel operations
void	irc_pass(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_nick(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_user(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_oper(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_quit(std::vector<std::string> cmd, Client* sender, Server* serv);
// Connection registration
void	irc_join(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_part(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_mode(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_topic(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_names(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_list(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_invite(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_kick(std::vector<std::string> cmd, Client* sender, Server* serv);
// Miscellaneous messages
void	irc_version(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_stats(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_time(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_admin(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_info(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_lusers(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_motd(std::vector<std::string> cmd, Client* sender, Server* serv);
// Sending messages
void	irc_privmsg(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_notice(std::vector<std::string> cmd, Client* sender, Server* serv);
// Server queries and commands
void	irc_who(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_whois(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_whowas(std::vector<std::string> cmd, Client* sender, Server* serv);
// User based queries
void	irc_kill(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_ping(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_pong(std::vector<std::string> cmd, Client* sender, Server* serv);
void	irc_error(Client* sender, std::string reason);
// Utils
void						parseArg(std::string cmdArg, std::vector<std::string>& argList);
std::vector<std::string>	vectorizator(std::string arg1, std::string arg2);
bool						ableToTalk(Client *client, Channel *channel);

#endif //~~ IRCSERV_H
