#ifndef SERVER_HPP
#define SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include "Client.hpp"
# include "Channel.hpp"

class Server
{
	private:

		int								_port;
		std::string						_password;
		bool							_online;
		time_t							_startTime;
		int								_sock;
		struct sockaddr_in				_servSocket;
		std::vector<struct pollfd>		_fds;
		std::map<std::string, Client*>	_clientsByName;
		std::map<int, Client*>			_clientsBySock;
		std::map<std::string, Client*>	_oldClients;
		std::map<std::string, Channel*>	_channels;

		void		acceptConnexions();
		void		receiveMessages();
		void		sendMessages();
		void		stop(int status); // SIGNAL HANDLING
		void		addClient(int sock);
		void		removeClient(Client *src, std::vector<struct pollfd>::iterator it);
	
	public:

		Server(int port, std::string password);
		~Server();

		std::string	getStartTime() const;
		void		run();
};

#endif
