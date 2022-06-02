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
# include <cerrno>
# include <climits>
# include "Server.hpp"

# define USAGE				"usage: ircserv <port> <password>"
# define SERV_NAME			"Potatoe's land"
# define SERV_VERSION		4.2
# define SERV_ADMIN			"pinkie_pie"
# define SERV_INFO			"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."
# define MAX_MESSAGE_LENGTH	512
# define MAX_PORT			65536

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
// enlever les clients déconnectés (quelle était l'utilité déjà ?)

// Problèmes actuels :

// plusieurs clients mal gérés
// couper le premier client connecté coupe le serveur