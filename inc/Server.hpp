#ifndef SERVER_H
#define SERVER_H

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <fstream>
# include <sstream>
# include "Client.hpp"
# include "Channel.hpp"

# define LOG_INFO		0
# define LOG_LISTEN		1
# define LOG_BROADCAST	2

typedef void (*command_t)(std::vector<std::string> cmd, Client* sender, Server* serv);

class Server
{
	public:

		Server(int port, std::string password);
		~Server();

		// SERVER
		int									getPort() const;
		std::string							getPassword() const;
		std::string							getStartTime() const;
		std::string							getCurrentTime() const;
		// CLIENTS
		Client*								getClient(std::string name) const;
		std::map<std::string, Client*> &	getAllClients();
		int									getOpsNumber();
		int									getNonRegisteredNumber();
		// std::map<std::string, Client*> &	getOldNickname();
		// CHANNELS
		Channel*							getChannel(std::string name) const;
		std::map<std::string, Channel*> &	getAllChannels();
		Channel*							newChannel(std::string name, Client* founder);
		// SERVER MAIN
		void								run();
	
	private:

		// SERVER
		int									_port;
		std::string							_password;
		bool								_online;
		time_t								_startTime;
		int									_fd;
		struct sockaddr_in					_servSocket;
		std::vector<struct pollfd>			_fdList;
		std::ofstream						_logFile;
		std::map<std::string, command_t>	_commands;
		// CONTENT
		std::map<std::string, Client*>		_clientsByName;
		std::map<int, Client*>				_clientsBySock;
		std::map<std::string, Client*>		_oldClients;
		std::map<std::string, Channel*>		_channels;
		// std::map<std::string, Client*>		_oldNickname;

		// SERVER MAIN
		void		acceptConnexions();
		void		receiveMessages();
		void		executeRequest(Client* sender);
		void		sendMessages();
		void		stop(int status); // SIGNAL HANDLING
		// SERVER UTILS
		void		initSupportedCommands();
		void		createLogFile();
		void		executeCommand(std::vector<std::string>	cmd, Client* sender);
		void		addClient(int sock);
		void		removeClient(Client *src);
		void		sendWelcome(Client* sender);
		// broadcast, listen
		void		addLog(std::string message, int type);
};

#endif //~~ SERVER_H
