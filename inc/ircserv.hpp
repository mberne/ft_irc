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
# include <vector>
# include "errors.hpp"
# include "rpl.hpp"

/*** ~~ DEFINES ~~ ***/

# define USAGE						"usage: ircserv <port> <password>"

# define SERV_NAME					"Potatoe's land"
# define SERV_VERSION				4.2
# define SERV_ADMIN					"pinkie_pie"
# define SERV_INFO					"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."

# define CHANNEL_LIMIT				25
# define CHANNEL_LIMIT_PER_CLIENT	10
# define MAX_NICKNAME_LENGTH		9
# define MAX_CHANNEL_LENGTH			50
# define MAX_TOPIC_LENGTH			390
# define MAX_MESSAGE_LENGTH			512
# define MAX_PORT					65536

/*** ~~ CLASSES ~~ ***/

class Server;
class Channel;
class Client;
# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"

/*** ~~ PROTOTYPES ~~ ***/

int main(int ac, char **av);

#endif //~~ IRCSERV_H

// A discuter :

// ne pas faire la commande STATS
// enlever les clients déconnectés (quelle était l'utilité déjà ?)
// limiter le nombre d'utilisateur et dire ciao aux losers
// classe commande : constructeur verifie cmd existe + nb argument via une map<string, int>
