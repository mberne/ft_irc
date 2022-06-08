#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "ircserv.hpp"
# include "Channel.hpp"

class Channel;

class Client
{
	public:

		Client(int socket);
		~Client(void);

		int				getSock(void) const;
		std::string		getNickname(void) const;
		void			setNickname(std::string nickname);
		std::string		getUser(void) const;
		void			setUser(std::string user);
		std::string		getHost(void) const;
		void			setHost(std::string host);
		std::string		getRealName(void) const;
		void			setRealName(std::string realName);
		bool			getOperator(void) const;
		void			setOperator(bool value);
		int				getNumberOfChannels(void) const;
		void			joinChannel(Channel* channel);
		Channel*		getChannel(std::string name);
		void			leaveChannel(Channel* channel);

		char*			getInputBuffer(void);
		char const *	getOutputBuffer(void) const;
		void			addToOutputBuffer(std::string output);
		void			clearOutputBuffer(void);

		bool			isOldNickname(std::string nickname);
		bool			isRegistered(void);

	private:

		int									_sock;
		std::string							_nickname;		// Unique nickname having a maximum length of nine (9) characters
		std::vector<std::string>			_oldNicknames;	// List of the old nicknames of the user
		std::string							_user;			// The username of the client on that host
		std::string							_host;			// The real name of the host that the client is running on
		std::string							_realName;		// The real name of the user
		bool								_op;			// If true the client is a server operator
		std::map<std::string, Channel*>		_channels;		// List of channels the client is connected to

		char								_inputBuffer[MAX_MESSAGE_LENGTH];	// 
		std::string							_outputBuffer;	//
};

#endif //~~ CLIENT_H
