#include "Client.hpp"

//~~ CONSTRUCTOR

Client::Client(int sock) : _fd(sock), _mods(0), _password(false), _connexionStartTime(time(NULL)), _lastCmdTime(time(NULL)) {}

//~~ DESTRUCTOR

Client::~Client() {}

//~~ CLIENT INFO

int				Client::getSock() const
{
	return _fd;
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
	_nickname = nickname.substr(0, NICKLEN);
}

// bool	Client::isOldNickname(std::string nickname)
// {
// 	for (std::vector<std::string>::iterator it = _oldNicknames.begin(); it != _oldNicknames.end(); it++)
// 		if (!nickname.compare(*it))
// 			return true;
// 	return false;
// }

std::string		Client::getUser() const
{
	if (_user.empty())
		return "*";
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

bool			Client::isServOperator() const
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

time_t	Client::getConnexionStartTime() const
{
	return _connexionStartTime;
}

time_t			Client::getLastCmdTime() const
{
	return _lastCmdTime;
}

void			Client::setLastCmdTime()
{
	_lastCmdTime = time(NULL);
}

bool	Client::getIsPing() const
{
	return _isPing;
}

void	Client::setIsPing(bool ping)
{
	_isPing = ping;
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
	if (isServOperator() == true)
		modsString += 'o';
	return modsString;
}

//~~ CHANNELS

void	Client::joinChannel(Channel* channel)	
{
	_channels.insert(std::make_pair(channel->getName(), channel));
	if (channel->getClient(_nickname) == NULL)
		channel->addClient(this);
}

void	Client::leaveChannel(Channel* channel)	
{
	_channels.erase(channel->getName());
	if (channel->getClient(_nickname) != NULL)
		channel->removeClient(this);
}

void		Client::leaveAllChannels(Server* serv)
{
	std::vector<std::string> cmd;

	cmd.push_back("PART");
	cmd.push_back("");
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it = _channels.begin())
	{
		cmd[1] = it->first;
		irc_part(cmd, this, serv);
	}
}

Channel*	Client::getChannel(std::string name) const
{
	if (_channels.find(name) == _channels.end())
		return (NULL);
	else
		return (_channels.find(name)->second);
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
		return _channels.begin()->second->getName();
}

std::string		Client::showChannelList()
{
	std::string		channelList;

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
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
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		it->second->sendToClients(msg);
}

//~~ BUFFER

std::string &	Client::getInputBuffer()
{
	return _inputBuffer;
}

void			Client::addToInputBuffer(const char* buf)
{
	_inputBuffer += buf;
}

const char*	Client::getOutputBuffer() const
{
	return _outputBuffer.c_str();
}

void	Client::addToOutputBuffer(std::string output)
{
	_outputBuffer += (output + "\n");
}

void	Client::clearOutputBuffer()
{
	_outputBuffer.clear();
}

bool	Client::hasOutput() const
{
	return !_outputBuffer.empty();
}
