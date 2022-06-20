#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "ircserv.hpp"
# include "Channel.hpp"

# define CLIENT_FLAG_O 1	// Server operator flag;
# define CLIENT_FLAG_I 2	// invisible client flag;

class Channel;

class Client
{
	public:

		Client(int sock);
		~Client();

		// CLIENT INFO
		int				getSock() const;
		std::string		getNickname() const;
		void			setNickname(std::string nickname);
		// bool			isOldNickname(std::string nickname);		// Return true if the string sent as parameter is an old client's nickname
		std::string		getUser() const;
		void			setUser(std::string user);
		std::string		getHost() const;
		void			setHost(std::string host);
		std::string		getRealName() const;
		void			setRealName(std::string realName);
		std::string		getPrefix() const;
		bool			isRegistered() const;						// Return true if the client used the registrations commands (PASS, NICK, USER)
		void			setPassword(bool proof);
		time_t			getConnexionStartTime() const;
		time_t			getLastCmdTime() const;
		void			setLastCmdTime();
		bool			getIsPing() const;
		void			setIsPing(bool ping);
		// MODS
		std::string		setMods(std::string mods);
		std::string		getMods() const;
		bool			hasMod(int mode) const;
		// CHANNEL
		void			joinChannel(Channel* channel);				// Add the Channel sent as paramater to the _channels map
		void			leaveChannel(Channel* channel);				// Remove the Channel sent as paramater from the _channels map
		void			leaveAllChannels(Server* serv);
		Channel*		getChannel(std::string name) const;
		int				getNumberOfChannels() const;
		std::string		getLastChannelName() const;
		std::string		showChannelList();
		void			sendToAllChannels(std::string msg);

		// BUFFER
		std::string &	getInputBuffer();
		void			addToInputBuffer(const char* buf);			// Append the string sent as parameter to the output buffer
		char const *	getOutputBuffer() const;
		void			addToOutputBuffer(std::string output);		// Append the string sent as parameter to the output buffer
		void			clearOutputBuffer();						// Clear the output buffer
		bool			hasOutput() const;							// Return true if the server have message to send to the client

	private:

		int									_fd;					// Socket of the client
		std::string							_nickname;				// Unique nickname
		std::vector<std::string>			_oldNicknames;			// List of the old nicknames of the user
		std::string							_user;					// The username of the client on that host
		std::string							_host;					// The real name of the host that the client is running on
		std::string							_realName;				// The real name of the user
		int									_mods;					// cf. comment at end of file
		std::map<std::string, Channel*>		_channels;				// List of channels the client is connected to
		bool								_password;				// If true the client has entered the server password using PASS command
		const time_t						_connexionStartTime;
		time_t								_lastCmdTime;
		bool								_isPing;

		std::string							_inputBuffer;			// Client's messages buffer
		std::string							_outputBuffer;			// Messages to Client buffer
};

#endif //~~ CLIENT_H

/*
*	USER MODS:
*   i - marks a users as invisible;
*   o - operator flag.
*/
