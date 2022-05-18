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
#include "Server.hpp"

#define USAGE			"usage: ircserv <port> <password>"
#define PROTOCOL		"tcp"

// #define FD_FREE			0
// #define FD_SERV			1
// #define FD_CLIENT		2

#define SERV_NAME		"Potatoe's land"
#define SERV_VERSION	4.2
#define SERV_ADMIN		"pinkie_pie"
#define SERV_INFO		"Made with *LOUD TRUCK SOUND* by pthomas and mberne in 2022. Copyleft."

typedef struct s_env
{
	Server				*serv;	// to be free
	struct protoent		*pe;
	struct sockaddr_in	servSocket;
}				t_env;
