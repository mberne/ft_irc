#ifndef IRCSERV_H
# define IRCSERV_H

/*** ~~ LIBS ~~ ***/

# include <cstdlib>
# include <string>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <poll.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <ctime>
# include <string>
# include <vector>
# include <map>
# include "errors.hpp"
# include "rpl.hpp"

/*** ~~ DEFINES ~~ ***/

# define USAGE						"usage: ircserv <port> <password>"

# define SERV_NAME					"Potatoe's land"
# define SERV_VERSION				4.2
# define SERV_ADMIN					"pinkie_pie"
# define SERV_ADMIN_EMAIL			"pinkie_pie@rainbow.dash"
# define SERV_INFO					"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."
# define SERV_LOC1					"Charbonniere, France"
# define SERV_LOC2					"42 Lyon"

# define CHANNEL_LIMIT				25
# define CHANNEL_LIMIT_PER_CLIENT	10
# define MAX_NICKNAME_LENGTH		9
# define MAX_CHANNEL_LENGTH			50
# define MAX_TOPIC_LENGTH			390
# define MAX_MESSAGE_LENGTH			512
# define MAX_PORT					65536

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

// main.cpp
int main(int ac, char **av);

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
// Sending messages
void	privmsg(std::vector<std::string> cmd, Client* sender, Server* serv);
void	notice(std::vector<std::string> cmd, Client* sender, Server* serv);
// Server queries and commands
void	who(std::vector<std::string> cmd, Client* sender, Server* serv);
void	whois(std::vector<std::string> cmd, Client* sender, Server* serv);
void	whowas(std::vector<std::string> cmd, Client* sender, Server* serv);
// User based queries
void	kill(std::vector<std::string> cmd, Client* sender, Server* serv);
void	ping(std::vector<std::string> cmd, Client* sender, Server* serv);
void	pong(std::vector<std::string> cmd, Client* sender, Server* serv);
void	error(std::vector<std::string> cmd, Client* sender, Server* serv);


#endif //~~ IRCSERV_H

// A discuter :

// ne pas faire la commande STATS
// enlever les clients déconnectés (quelle était l'utilité déjà ?)
// limiter le nombre d'utilisateur et dire ciao aux losers
// classe commande : constructeur verifie cmd existe + nb argument via une map<string, int>
