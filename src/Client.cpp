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
	_nickname = nickname.substr(0, NICK_LEN);
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

std::string		Client::setMods(std::string mods)
{
	std::cout << mods << std::endl;
	std::string		charset = "oi";
	int				flag = 1;

	for(size_t	i = 0; i < charset.size(); i++)
	{
		if (mods.find(charset[i]) != std::string::npos)
		{
			if (mods[mods.find(charset[i]) - 1] == '+')
				_mods |= flag;
			if (mods[mods.find(charset[i]) - 1] == '-')
				_mods &= ~flag;
		}
		flag <<= 1;
	}
	return (mods);
}

std::string		Client::getMods() const
{
	std::string modsString("+");

	if (hasMod(CLIENT_FLAG_I) == true)
		modsString += "i";
	if (hasMod(CLIENT_FLAG_O) == true)
		modsString += "o";
	return modsString;
}

bool			Client::hasMod(int mode) const
{
	return ((_mods | mode) == _mods);
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
		else if (it->second->hasMod(CHANNEL_FLAG_M) && it->second->hasVoice(this))
			channelList += "+";
		channelList += it->second->getName();
	}
	return channelList;
}

void			Client::sendToAllChannels(std::string msg, Client* sender)
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		it->second->sendToClients(msg, sender);
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
