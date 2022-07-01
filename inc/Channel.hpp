#ifndef CHANNEL_H
# define CHANNEL_H

/*** ~~ HEADERS ~~ ***/
// Standard libs
# include <string>
# include <vector>
# include <map>
// Classes
# include "ircserv.hpp"
# include "Server.hpp"
# include "Client.hpp"

/*** ~~ DEFINES ~~ ***/
// Channel modes
# define CHANNEL_FLAG_P 1	// Private channel flag;
# define CHANNEL_FLAG_S 2	// Secret channel flag;
# define CHANNEL_FLAG_I 4	// Invite-only channel flag;
# define CHANNEL_FLAG_T 8	// Topic settable by channel operator only flag;
# define CHANNEL_FLAG_N 16	// No messages to channel from clients on the outside flag;
# define CHANNEL_FLAG_M 32	// Moderated channel flag;

/*** ~~ CLASSES ~~ ***/

class Client;

class	BanMask
{
	public:

		BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos);
		~BanMask();

		bool			isClientBanned(Client* client) const;
		std::string		getBanMask() const;

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
		~Channel();

		// CHANNEL INFO
		std::string						getName() const;
		std::string						getTopic() const;
		void							setTopic(std::string topic);
		std::string						getTopicTime() const;
		void							setTopicTime();
		std::string						getTopicWriter() const;
		void							setTopicWriter(std::string writer);
		std::string						getPassword() const;
		int								clientCount() const;
		int								getUserLimit() const;
		std::map<std::string, BanMask>&	getBanList();
		// MODES
		std::string		setModes(std::string modes, std::map<char, std::string>& modesArgs);
		bool			hasModes(mode_t modes) const;
		std::string		getModes() const;
		void			addOperator(Client* client);			// Add the Client to the list of channel operators
		void			removeOperator(Client* client);			// Remove the Client from the list of channel operators
		void			addClientWithVoice(Client* client);		// Add the Client to the list of clients with voice permission
		void			removeClientWithVoice(Client* client);	// Remove the Client from the list of clients with voice permission
		void			addInvitedClient(Client* client);		// Add the Client to the list of clients invited
		void			removeInvitedClient(Client* client);	// Remove the Client from the list of clients invited
		void			addBanMask(std::string banMask);
		void			removeBanMask(std::string banMask);
		// CLIENTS
		void								addClient(Client* client);						// Add the Client to the channel
		void								removeClient(Client* client, Server* serv);		// Remove the Client from the channel
		Client*								getClient(std::string name) const;
		bool								isOperator(Client* client) const;
		bool								hasVoice(Client* client) const;					// Return true if the client has voice permission
		bool								isBanned(Client* client);
		bool								isInvited(Client* client) const;
		bool								ableToTalk(Client *client);
		std::string							showClientsList();
		void								sendToClients(std::string msg, Client* sender);
		std::map<std::string, Client*> &	getAllClients();

	private:

		std::string const					_name;			// cf. comment at end of file
		std::string							_topic;			// Topic of the channel
		time_t								_topicTime;
		std::string							_topicWriter;
		std::string							_password;		// Channek's password
		int									_userLimit;		// Maximum of clients that can be connected to the channel
		mode_t								_modes;			// cf. comment at end of file

		std::map<std::string, Client*>		_clients;				// List of clients connected to the channel
		std::map<std::string, Client*>		_operators;				// List of channel's operators
		std::map<std::string, Client*>		_clientsWithVoicePerm;	// List of clients with voice permission on (only if flag m is on)
		std::map<std::string, Client*>		_invitedClients;		// List of clients that are invited to the channel
		std::map<std::string, BanMask>		_banList;				// List of ban masks

		void	addModes(char mode, std::map<char, std::string>& modesArgs, bool& change);
		void	removeModes(char mode, std::map<char, std::string>& modesArgs, bool& change);
};

#endif //~~ CHANNEL_H

/*
*	NAME RESTRICTIONS:
*	Channels names are strings (beginning with a '&' or '#' character) of length up to 200 characters.
*	Apart from the requirement that the first character being either '&' or '#';
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
