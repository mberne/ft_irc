#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "ircserv.hpp"
# include "Channel.hpp"

# define CHAN_O 1	// Server operator flag;
# define CHAN_I 2	// invisible client flag;

class Channel;

class Client
{
	public:

		Client(int sock);
		~Client();

		int				getSock() const;
		std::string		getNickname() const;
		void			setNickname(std::string nickname);
		std::string		getUser() const;
		void			setUser(std::string user);
		std::string		getHost() const;
		void			setHost(std::string host);
		std::string		getRealName() const;
		void			setRealName(std::string realName);
		bool			isOperator() const;
		void			setOperator(bool value);
		int				getNumberOfChannels() const;
		Channel*		getChannel(std::string name);
		char*			getInputBuffer();
		char const *	getOutputBuffer() const;

		void			joinChannel(Channel* channel);				// Add the Channel sent as paramater to the _channels map
		void			leaveChannel(Channel* channel);				// Remove the Channel sent as paramater from the _channels map
		std::string		getLastChannelName();
		void			addToOutputBuffer(std::string output);		// Append the string sent as parameter to the output buffer
		void			clearOutputBuffer();						// Clear the output buffer
		bool			hasOutput();								// Return true if the server have message to send to the client
		bool			isOldNickname(std::string nickname);		// Return true if the string sent as parameter is an old client's nickname
		bool			isRegistered();								// Return true if the client used the registrations commands (PASS, NICK, USER)

	private:

		int									_sock;					// Socket of the client
		std::string							_nickname;				// Unique nickname
		std::vector<std::string>			_oldNicknames;			// List of the old nicknames of the user
		std::string							_user;					// The username of the client on that host
		std::string							_host;					// The real name of the host that the client is running on
		std::string							_realName;				// The real name of the user
		bool								_op;					// If true the client is a server operator
		std::map<std::string, Channel*>		_channels;				// List of channels the client is connected to
		bool								_hasEnteredPassword;	// If true the client has entered the server password using PASS command

		char								_inputBuffer[MAX_MESSAGE_LENGTH];	// Client's messages buffer
		std::string							_outputBuffer;						// Messages to Client buffer
};

#endif //~~ CLIENT_H
