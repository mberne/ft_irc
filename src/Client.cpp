#include "Client.hpp"

//~~ CONSTRUCTOR

Client::Client(int sock) : _sock(sock) {}

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
	_nickname = nickname.substr(0, MAX_NICKNAME_LENGTH);
}


std::string		Client::getHost(void) const
{
	return(_host);
}

void			Client::setHost(std::string host)
{
	_host = host;
}

std::string		Client::getUser(void) const
{
	return(_user);
}

void			Client::setUser(std::string user)
{
	_user = user;
}

std::string		Client::getRealName(void) const
{
	return(_realName);
}

void			Client::setRealName(std::string realName)
{
	_realName = realName;
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

void	Client::joinChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.insert(std::pair<std::string, Channel*>(channel->getName(), channel));
}

Channel*	Client::getChannel(std::string name)
{
	return (_channels.find(name)->second);
}

void	Client::leaveChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.erase(channel->getName());
}

char*		Client::getInputBuffer( void )
{
	return (_inputBuffer);
}

const char*	Client::getOutputBuffer( void ) const
{
	return (_outputBuffer.c_str());
}

//~~ METHODS

bool	Client::isOldNickname(std::string nickname)
{
	for (std::vector<std::string>::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
		if (!nickname.compare(*it))
			return (true);
	return (false);
}

void			Client::clearOutputBuffer(void)
{
	_outputBuffer.empty();
}
