#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <string>
#include <cstdlib>

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

#define SERVER_NAME		"Potato's land"
#define SERVER_VERSION	4.2
#define SERVER_ADMIN	"Pinkie Pie"
#define SERVER_INFO		"Made with love by pthomas and mberne in 2022. Copyright damn son."

typedef struct irc
{
	Server			*serv;
	struct protoent	*pe;
	// ?
}				t_struct irc;
