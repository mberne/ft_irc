#ifndef SERVER_H
#define SERVER_H

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include "Client.hpp"
# include "Channel.hpp"
# include "Command.hpp"

class Server
{
	public:

		Server(int port, std::string password);
		~Server();

		int									getPort() const;
		std::string							getPassword() const;
		std::string							getStartTime() const;
		Client*								getClient(std::string name) const;
		Channel*							getChannel(std::string name) const;
		std::map<std::string, Client*> &	getAllClients();
		std::map<std::string, Channel*> &	getAllChannels();

		void		run();
		std::string	currentTime();
	
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
		std::vector<Command>			_cmdList;

		void		addClient(int sock);
		void		removeClient(Client *src, std::vector<struct pollfd>::iterator it);

		void		initSupportedCommands();
		void		acceptConnexions();
		void		receiveMessages();
		void		handleRequests(Client* client);
		void		executeRequest(Client* sender);
		void		executeCommand(std::vector<std::string>	cmd, Client* sender);
		void		sendMessages();
		void		stop(int status); // SIGNAL HANDLING
};

#endif //~~ SERVER_H
