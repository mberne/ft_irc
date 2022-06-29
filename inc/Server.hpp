#ifndef SERVER_H
#define SERVER_H

/*** ~~ HEADERS ~~ ***/
// Standard libs
# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <list>
# include <csignal>
# include <fstream>
# include <ctime>
# include <algorithm>
// Other libs
# include <unistd.h>
# include <netdb.h>
# include <netinet/tcp.h>
# include <poll.h>
# include <arpa/inet.h>
# include <fcntl.h>
// Classes
# include "ircserv.hpp"
# include "Channel.hpp"
# include "Client.hpp"
// Replies
# include "errors.hpp"
# include "replies.hpp"

/*** ~~ DEFINES ~~ ***/
// Logs options
# define LOG_INFO		0
# define LOG_LISTEN		1
# define LOG_BROADCAST	2
# define LOG_MESSAGE	3
# define LOG_ERROR		4

/*** ~~ TYPES ~~ ***/

typedef void (*command_t)(std::vector<std::string> cmd, Client* sender, Server* serv);

/*** ~~ CLASS ~~ ***/

class Server
{
	public:

		Server(int port, std::string password);
		~Server();

		// SERVER
		void								run();
		void								stop(int status);
		in_port_t							getPort() const;
		std::string							getPassword() const;
		std::string							getStartTime() const;
		std::string							getCurrentTime() const;
		void								addLog(std::string message, mode_t type);
		// CLIENT
		Client*										getClient(std::string name) const;
		std::map<std::string, Client*>&				getAllClients();
		size_t										getOpsNumber();
		size_t										getNonRegisteredNumber();
		void										addOldNickname(Client* client);
		std::list<std::vector<std::string> >&		getOldNicknames();
		void										addClient(int sock);
		void										removeClient(Client *src);
		void										pingClient(Client* client);
		// CHANNEL
		Channel*							getChannel(std::string name) const;
		std::map<std::string, Channel*>&	getAllChannels();
		Channel*							addChannel(std::string name, Client* founder);
		void								removeChannel(Channel* channel);
	
		static bool		online;

	private:

		// SERVER
		int									_port;
		std::string							_password;
		time_t								_startTime;
		int									_fd;
		struct sockaddr_in					_servSocket;
		std::vector<struct pollfd>			_fdList;
		std::ofstream						_logFile;
		std::map<std::string, command_t>	_commands;
		// CONTENT
		std::map<std::string, Client*>				_clientsByName;
		std::map<int, Client*>						_clientsBySock;
		std::list< std::vector<std::string> >		_oldNicknames;
		std::map<std::string, Channel*>				_channels;

		// SERVER MAIN
		void		acceptConnexions();
		void		receiveMessages();
		void		executeRequest(Client* sender);
		void		sendMessages();
		// SERVER UTILS
		void		initSupportedCommands();
		void		executeCommand(std::vector<std::string>	cmd, Client* sender);
		void		sendWelcome(Client* sender);
};

#endif //~~ SERVER_H
