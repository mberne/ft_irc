#include "Client.hpp"

//~~ CONSTRUCTOR

Client::Client(int socket) : _sock(socket) {}

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

std::string		Client::getLastChannelName(void)
{
	if (!_channels.size())
		return ("*");
	else
		return (_channels.at(0)->getName());
}

void	Client::leaveChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.erase(channel->getName());
}

char*		Client::getInputBuffer(void)
{
	return (_inputBuffer);
}

const char*	Client::getOutputBuffer(void) const
{
	return (_outputBuffer.c_str());
}

void	Client::addToOutputBuffer(std::string output)
{
	_outputBuffer += output;
}

bool	Client::hasOutput()
{
	return (!_outputBuffer.empty());
}

void	Client::clearOutputBuffer(void)
{
	_outputBuffer.clear();
}

//~~ METHODS

bool	Client::isOldNickname(std::string nickname)
{
	for (std::vector<std::string>::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
		if (!nickname.compare(*it))
			return (true);
	return (false);
}

bool	Client::isRegistered(void)
{
	return (_nickname.empty() == false && _user.empty() == false);
}
