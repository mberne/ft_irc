#include "Client.hpp"

//~~ CONSTRUCTOR

Client::Client(int sock) : _sock(sock), _mods(0), _password(false) {}

//~~ DESTRUCTOR

Client::~Client() {}

//~~ CLIENT INFO

int				Client::getSock() const
{
	return _sock;
}

std::string		Client::getNickname() const
{
	if (_nickname.empty())
		return "*";
	return _nickname;
}

void			Client::setNickname(std::string nickname)
{
	_oldNicknames.push_back(_nickname);
	_nickname = nickname.substr(0, MAX_NICKNAME_LENGTH);
}

bool	Client::isOldNickname(std::string nickname)
{
	for(std::vector<std::string>::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
		if (!nickname.compare(*it))
			return true;
	return false;
}

std::string		Client::getUser() const
{
	return _user;
}

void			Client::setUser(std::string user)
{
	_user = user;
}

std::string		Client::getHost() const
{
	return _host;
}

void			Client::setHost(std::string host)
{
	_host = host;
}

std::string		Client::getRealName() const
{
	return _realName;
}

void			Client::setRealName(std::string realName)
{
	_realName = realName;
}

std::string		Client::getPrefix() const
{
	return (":" + getNickname() + "!" + getUser() + "@" + getHost());
}

bool	Client::isRegistered() const
{
	return (_nickname.empty() == false && _user.empty() == false && _password == true);
}

bool			Client::isOperator() const
{
	return ((_mods | CLIENT_FLAG_O) == _mods);
}

bool			Client::isInvisible() const
{
	return ((_mods | CLIENT_FLAG_I) == _mods);
}

void	Client::setPassword(bool proof)
{
	_password = proof;
}

//~~ MODS

void			Client::addMods(std::string mods)
{
	if (mods.find('o'))
		_mods |= CLIENT_FLAG_O;
	if (mods.find('i'))
		_mods |= CLIENT_FLAG_I;
}

void			Client::removeMods(std::string mods)
{
	if (mods.find('o'))
		_mods &= ~CLIENT_FLAG_O;
	if (mods.find('i'))
		_mods &= ~CLIENT_FLAG_I;
}

std::string		Client::getMods() const
{
	std::string modsString("+");

	if (isInvisible() == true)
		modsString += 'i';
	if (isOperator() == true)
		modsString += 'o';
	return modsString;
}

//~~ CHANNELS

void	Client::joinChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.insert(std::make_pair(channel->getName(), channel));
}

void	Client::leaveChannel(Channel* channel)	
{
	// Error handling needed!
	_channels.erase(channel->getName());
}

Channel*	Client::getChannel(std::string name) const
{
	return _channels.find(name)->second;
}

int				Client::getNumberOfChannels() const
{
	return _channels.size();
}

std::string		Client::getLastChannelName() const
{
	if (!_channels.size())
		return "*";
	else
		return _channels.at(0)->getName();
}

std::string		Client::showChannelList()
{
	std::string		channelList;

	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it != _channels.begin())
			channelList += " ";
		if (it->second->isOperator(this))
			channelList += "@";
		else if (it->second->isModerated() && it->second->hasVoice(this))
			channelList += "+";
		channelList += it->second->getName();
	}
	return channelList;
}

void			Client::sendToAllChannels(std::string msg)
{
	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		it->second->sendToClients(msg);
}

//~~ BUFFER

std::string &	Client::getInputBuffer()
{
	return _inputBuffer;
}

void			Client::addToInputBuffer(char* buf)
{
	_inputBuffer += buf;
}

const char*	Client::getOutputBuffer() const
{
	return _outputBuffer.c_str();
}

void	Client::addToOutputBuffer(std::string output)
{
	_outputBuffer += output;
}

void	Client::clearOutputBuffer()
{
	_outputBuffer.clear();
}

bool	Client::hasOutput() const
{
	return !_outputBuffer.empty();
}
