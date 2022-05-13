#ifndef SERVER_HPP
#define SERVER_HPP

# include <string>
# include <map>
# include "Client.hpp"
# include "Channel.hpp"

class Server
{
	private:

		int			_port;
		std::string	_password;
		std::string	_stats; // tous les trucs STATS ?
	
	public:

		std::map<std::string, Client*>	clients;
		std::map<std::string, Client*>	disconnectClients;
		std::map<std::string, Channel*>	channels;

		Server(int port, std::string password);
		~Server();

		std::string	getPort();
		std::string	getPassword();
		// get stats ?
};

#endif
