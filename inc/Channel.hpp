#ifndef CHANNEL_H
# define CHANNEL_H

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "Client.hpp"

/*** ~~ CHANNEL MOD FLAGS ~~ ***/

# define CHAN_P 1	// Private channel flag;
# define CHAN_S 2	// Secret channel flag;
# define CHAN_I 4	// Invite-only channel flag;
# define CHAN_T 8	// Topic settable by channel operator only flag;
# define CHAN_N 16	// No messages to channel from clients on the outside flag;
# define CHAN_M 32	// Moderated channel flag;

class Client;

class	BanMask
{
	public:

		BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos);
		~BanMask(void);

		bool			isClientBanned(Client* client) const;

		static bool		stringCorrespondToMask(std::string str, std::string mask);

	private:

		std::string const	_mask;
		std::string const	_nick;
		std::string const	_ident;
		std::string const	_host;
};

class Channel
{
	public:

		Channel(std::string name);
		~Channel(void);

		std::string		getName(void) const;
		std::string		getTopic(void) const;
		void			setTopic(std::string topic);
		std::string		getPassword(void) const;
		void			setPassword(std::string topic);
		int				getUserLimit(void) const;
		void			setUserLimit(int topic);
		int				getMods(void) const;
		void			setMods(int topic);

		void			addClient(Client* client);
		void			removeClient(Client* client);
		void			addOperator(Client* client);
		void			removeOperator(Client* client);
		void			addclientWithVoice(Client* client);
		void			removeclientWithVoice(Client* client);
		void			addBanMask(std::string banMask);
		void			removeBanMask(std::string banMask);

		bool			isConnected(Client* client) const;
		bool			isOperator(Client* client) const;
		bool			hasVoice(Client* client) const;
		bool			isBanned(Client* client);

		int				clientCount(void) const;

	private:

		std::string const					_name;			// cf. comment at end of file
		std::string							_topic;			// Topic of the channel
		std::string							_password;		// Channek's password
		int									_userLimit;		// Maximum of clients that can be connected to the channel
		int									_mods;			// cf. comment at end of file

		std::map<std::string, Client*>		_clients;				// List of clients connected to the channel
		std::map<std::string, Client*>		_operators;				// List of channel's operators
		std::map<std::string, Client*>		_clientsWithVoicePerm;	// List of clients with voice permission on (only if flag m is on)
		std::map<std::string, BanMask>		_banList;	// List of clients with voice permission on (only if flag m is on)
};

#endif //~~ CHANNEL_H

/*
*	NAME RESTRICTIONS:
*	Channels names are strings (beginning with a '&' or '#' character) of length up to 200 characters.
*	Apart from the the requirement that the first character being either '&' or '#';
*	the only restriction on a channel name is that it may not contain any spaces (' '),
*	a control G (^G or ASCII 7)
*	or a comma (',' which is used as a list item separator by the protocol).
*	
*	CHANNEL MODES:
*	i - toggle the invite-only channel flag;
*	m - toggle the moderated channel flag;
*	n - toggle the no messages to channel from clients on the outside flag;
*	p - toggle the private channel flag;
*	s - toggle the secret channel flag;
*	t - toggle the topic settable by channel operator only flag;
*
*/
