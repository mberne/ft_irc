#ifndef IRCSERV_H
# define IRCSERV_H

/*** ~~ LIBS ~~ ***/

# include <cstdlib>
# include <stdlib.h>
# include <string>
# include <cstring>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <poll.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <ctime>
# include <string>
# include <vector>
# include <map>
# include <cerrno>
# include <cstdio>
# include "errors.hpp"
# include "replies.hpp"

/*** ~~ DEFINES ~~ ***/

# define USAGE						"usage: ircserv <port> <password>"

// Server info
# define SERV_NAME					"potatoes.land"
# define SERV_VERSION				"4.2"
# define SERV_CREATION				"Thursday 9 June 2022 at 10:42:00 UTC"
# define SERV_ADMIN					"Pinkie Pie"
# define SERV_ADMIN_EMAIL			"pinkie_pie@rainbow.dash"
# define SERV_INFO					"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."
# define SERV_LOC1					"Charbonniere-Les-Bains, France"
# define SERV_LOC2					"42 Lyon"
# define PROMPT(num, user)			std::string(":") + SERV_NAME + std::string(num) + user

// Server specifications
// Required
# define CASEMAPPING				"ascii"
# define CHANLIMIT					10
# define CHANMODES					"opsitnmlbvk"
# define CHANNELLEN					50
# define CHANTYPES					"#"
# define ELIST						""
# define EXECPTS
# define EXTBAN
# define HOSTLEN					10
# define INVEX
# define KICKLEN					256
# define MAXLIST
# define MAXTARGETS
# define MODES
# define NETWORK
# define NICKLEN					20
# define PREFIX
# define SAFELIST
# define SILENCE
# define STATUSMSG
# define TARGMAX					512
# define TOPICLEN					400
# define USERLEN					10
// Others
# define MESSAGELEN					512
# define REALNAMELEN				50
# define ASCII_CHARSET				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define CRLF						std::string("\r\n")
# define CLIENT_LIMIT				100
# define CHANNEL_LIMIT				1000
# define MAX_PORT					65536
# define MAX_TIME_AFK				300
# define MAX_PING_TIME				60
# define OPERATOR_USER				"p1k13p13"
# define OPERATOR_PASSWORD			"*_*SEGFAULT_11"
# define CLIENT_LIMIT_PER_CHANNEL	100

/*** ~~ CLASSES ~~ ***/

class Server;
class Client;
class Channel;
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

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
void	irc_error(std::vector<std::string> cmd, Client* sender, Server* serv);
// utils
void	parseArg(std::string cmdArg, std::vector<std::string>& argList);

#endif //~~ IRCSERV_H