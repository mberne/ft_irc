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
	private:

		int							_port;
		std::string					_password;
		std::string					_stats; // tous les trucs STATS ?
		std::vector<Command>		_cmdList;

		void		initSupportedCommands();
		void		acceptConnexions();
		void		receiveMessages();
		void		executeRequest(std::string cmdLine, Client* sender);
		void		sendMessages();
		



	public:

		struct sockaddr_in				servSocket;
		std::vector<struct pollfd>		fds;
		int								sock;
		bool							online;
		std::map<std::string, Client*>	clientsByName;
		std::map<int, Client*>			clientsBySock;
		std::map<std::string, Client*>	oldClients;
		std::map<std::string, Channel*>	channels;			

		Server(int port, std::string password);
		~Server();

		void		run();
		void		stop(int status); // SIGNAL HANDLING

		int			getPort() const;
		std::string	getPassword() const;
		// get stats ?
		void		addClient(int sock);
		void		removeClient(Client *src, std::vector<struct pollfd>::iterator it);
};

#endif //~~ SERVER_H
