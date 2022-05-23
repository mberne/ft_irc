#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "Server.hpp"

#define USAGE				"usage: ircserv <port> <password>"
#define PROTOCOL			"tcp"
#define SERV_NAME			"Potatoe's land"
#define SERV_VERSION		4.2
#define SERV_ADMIN			"pinkie_pie"
#define SERV_INFO			"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."
#define MAX_MESSAGE_LENGTH	512

typedef struct s_env
{
	Server						*serv;
	struct protoent				*pe;
	struct sockaddr_in			servSocket;
	std::vector<struct pollfd>	fds;
}				t_env;
