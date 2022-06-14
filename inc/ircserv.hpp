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
# define PROMPT(num, user)			SERV_NAME + std::string(num) + user

// Server specifications
// Required
# define CASEMAPPING				"ascii"
# define CHANLIMIT					10
# define CHANMODES					"opsitnmlbvk"
# define CHANNELLEN					50
# define CHANTYPES					"#"
# define ELIST //à faire
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
# define TARGMAX
# define TOPICLEN					400
# define USERLEN					10
// Others
# define MESSAGELEN					512
# define REALNAMELEN				50
# define NICKNAME_CHARSET			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define CRLF						std::string("\r\n")
# define CHANNEL_LIMIT_PER_CLIENT	10
# define CLIENT_LIMIT				100
# define CHANNEL_LIMIT				1000
# define MAX_PORT					65536
# define MAX_TIME_AFK				300
# define MAX_PING_TIME				60

/*** ~~ CLASSES ~~ ***/

class Server;
class Client;
class Channel;
class Command;
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Command.hpp"

/*** ~~ PROTOTYPES ~~ ***/

// Server commands
// Channel operations
void	pass(std::vector<std::string> cmd, Client* sender, Server* serv);
void	nick(std::vector<std::string> cmd, Client* sender, Server* serv);
void	user(std::vector<std::string> cmd, Client* sender, Server* serv);
void	oper(std::vector<std::string> cmd, Client* sender, Server* serv);
void	quit(std::vector<std::string> cmd, Client* sender, Server* serv);
// Connection registration
void	join(std::vector<std::string> cmd, Client* sender, Server* serv);
void	part(std::vector<std::string> cmd, Client* sender, Server* serv);
void	mode(std::vector<std::string> cmd, Client* sender, Server* serv);
void	topic(std::vector<std::string> cmd, Client* sender, Server* serv);
void	names(std::vector<std::string> cmd, Client* sender, Server* serv);
void	list(std::vector<std::string> cmd, Client* sender, Server* serv);
void	kick(std::vector<std::string> cmd, Client* sender, Server* serv);
// Miscellaneous messages
void	version(std::vector<std::string> cmd, Client* sender, Server* serv);
void	stats(std::vector<std::string> cmd, Client* sender, Server* serv);
void	time(std::vector<std::string> cmd, Client* sender, Server* serv);
void	admin(std::vector<std::string> cmd, Client* sender, Server* serv);
void	info(std::vector<std::string> cmd, Client* sender, Server* serv);
void	lusers(std::vector<std::string> cmd, Client* sender, Server* serv);
void	motd(std::vector<std::string> cmd, Client* sender, Server* serv);
// Sending messages
void	privmsg(std::vector<std::string> cmd, Client* sender, Server* serv);
void	notice(std::vector<std::string> cmd, Client* sender, Server* serv);
// Server queries and commands
void	who(std::vector<std::string> cmd, Client* sender, Server* serv);
void	whois(std::vector<std::string> cmd, Client* sender, Server* serv);
void	whowas(std::vector<std::string> cmd, Client* sender, Server* serv);
// User based queries
void	kill(std::vector<std::string> cmd, Client* sender, Server* serv);
// void	ping(std::vector<std::string> cmd, Client* sender, Server* serv);
void	pong(std::vector<std::string> cmd, Client* sender, Server* serv);
// void	error(std::vector<std::string> cmd, Client* sender, Server* serv);
// Welcome command
void	sendWelcome(Client* sender, Server* serv);

#endif //~~ IRCSERV_H