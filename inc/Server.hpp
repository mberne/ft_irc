#ifndef SERVER_HPP
#define SERVER_HPP

# include <string>
# include <map>
# include "Client.hpp"
# include "Channel.hpp"

#define SERVER_NAME "Potato's land"
#define SERVER_VERSION 4.2
#define SERVER_ADMIN "Pinkie Pie"
#define SERVER_INFO	"Made with love by pthomas and mberne in 2022. Copyright damn son."

class Server
{
	private:

		std::string		_port;
		std::string		_password;
		std::string		_stats; // tous les trucs STATS ?
	
	public:

		std::map<std::string, Client*> clients;
		std::map<std::string, Client*> disconnectClients;
		std::map<std::string, Channel*> channels;

		Server(int port, std::string password);
		~Server();

		std::string	getPort();
		std::string	getPassword();
		// get stats ?
};

#endif
