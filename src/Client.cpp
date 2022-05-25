#include "Client.hpp"

int		Client::channelsLimit = 10;
int		Client::nicknameMaxLength = 9;

//~~ CONSTRUCTOR

Client::Client(int sock, std::string nickname, std::string user, std::string host, std::string realName) \
				: _sock(sock), _nickname(nickname.substr(0, nicknameMaxLength)), _user(user), _host(host), _realName(realName), _op(false) {}

//~~ DESTRUCTOR

Client::~Client(void) {}

//~~ ACCESSOR

int				Client::getSock(void) const
{
	return(_sock);
}

std::string		Client::getNickname(void) const
{
	return(_nickname);
}

void			Client::setNickname(std::string nickname)
{
	_oldNicknames.push_back(_nickname);
	_nickname = nickname.substr(0, nicknameMaxLength);
}


std::string		Client::getHost(void) const
{
	return(_host);
}

std::string		Client::getUser(void) const
{
	return(_user);
}

std::string		Client::getRealName(void) const
{
	return(_realName);
}

bool			Client::getOperator(void) const
{
	return (_op);
}

void			Client::setOperator(bool value)
{
	_op = value;
}

int				Client::getNumberOfChannels(void) const
{
	return (_channels.size());
}

//~~ METHODS

bool	Client::isOldNickname(std::string nickname)
{
	for (std::vector<std::string>::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
		if (!nickname.compare(*it))
			return (true);
	return (false);
}

void	Client::joinChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.insert(std::pair<std::string, Channel*>(channel->getName(), channel));
}

void	Client::leaveChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.erase(channel->getName());
}
