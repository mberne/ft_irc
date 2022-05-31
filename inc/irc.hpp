#ifndef IRC_HPP
#define IRC_HPP

# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <signal.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <poll.h>
# include <cstring>
# include <cstdlib>
# include <vector>
# include "Server.hpp"

# define USAGE				"usage: ircserv <port> <password>"
# define PROTOCOL			"tcp"
# define SERV_NAME			"Potatoe's land"
# define SERV_VERSION		4.2
# define SERV_ADMIN			"pinkie_pie"
# define SERV_INFO			"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."
# define MAX_MESSAGE_LENGTH	512

typedef struct s_env
{
	Server						*serv;
	struct sockaddr_in			servSocket;
	std::vector<struct pollfd>	fds;
}				t_env;

int main(int ac, char **av);
int	mainLoop(t_env *irc);

#endif

// A discuter :

// ne pas faire la commande STATS
// ne pas faire de map avec le nom d'utilisateur mais avec le socket du client, voire juste un vecteur
// enlever le define du protocole (utilisé qu'une fois et de toute façon, pas pratique à changer si define quand même car fct vont crash)
// enlever les clients déconnectés (quelle était l'utilité déjà ?)

// Problèmes actuels :

// plusieurs clients et paf : le zbeul