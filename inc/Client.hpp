#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "Channel.hpp"

class Channel;

class Client
{
	public:

		Client(std::string nickname, std::string user, std::string host, std::string realName);
		~Client(void);

		std::string		getNickname(void) const;
		void			setNickname(std::string nickname);
		std::string		getUser(void) const;
		std::string		getHost(void) const;
		std::string		getRealName(void) const;
		bool			getOperator(void) const;
		void			setOperator(bool value);
		int				getNumberOfChannels(void) const;
		
		bool			isOldNickname(std::string nickname);
		void			joinChannel(Channel* channel);
		void			leaveChannel(Channel* channel);

	private:

		std::string							_nickname;		// Unique nickname having a maximum length of nine (9) characters
		std::vector<std::string>			_oldNicknames;	// List of the old nicknames of the user
		std::string const					_user;			// The username of the client on that host
		std::string const					_host;			// The real name of the host that the client is running on
		std::string const					_realName;		// The real name of the user
		bool								_op;			// If true the client is a server operator
		std::map<std::string, Channel*>		_channels;		// List of channels the client is connected to

		static int							channelsLimit;	// Maximum of channels the client can connect to
		static int							nicknameMaxLength;	// Maximum of channels the client can connect to

		// I think the following is only if we have a server network in doubt i let it here
		// std::string		_server; // The server to which the client is connected
		// If you uncomment the previous statement don't forget to add it in every method like constructors and others

};

#endif //~~ CLIENT_H
