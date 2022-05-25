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

		int								sock;
		bool							online;
		std::map<std::string, Client*>	clientsByName;
		std::map<int, Client*>			clientsBySocket;
		std::map<std::string, Client*>	disconnectClients;
		std::map<std::string, Channel*>	channels;

		Server(int port, std::string password);
		~Server();

		int			getPort() const;
		std::string	getPassword() const;
		// get stats ?
		// void		addClient(int sock);
		// void		removeClient(Client *src, int it);
};

#endif
