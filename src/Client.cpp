#include "Client.hpp"

//~~ CONSTRUCTOR

Client::Client(int sock) : _fd(sock), _modes(0), _connexionStartTime(time(NULL)), _password(false), _retriesLeft(MAX_PASS_ATTEMPT), _lastCmdTime(time(NULL)) {}

Client::Client(std::vector<std::string>	ident) : _nickname(ident[0]), _user(ident[1]), _host(ident[2]), _realName(ident[3]), _connexionStartTime(0) {}

//~~ DESTRUCTOR

Client::~Client() {}

//~~ CLIENT INFO

int				Client::getSock() const
{
	return _fd;
}

std::string		Client::getNickname() const
{
	return (_nickname.empty() ? "*" : _nickname);
}

void			Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

std::string		Client::getUser() const
{
	return (_user.empty() ? "*" : _user);
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

//~~ MODES

std::string		Client::setModes(std::string modes)
{
	std::string		charset = "oi";
	mode_t			flag = 1;

	for (size_t	i = 0; i < charset.size(); i++)
	{
		if (modes.find(charset[i]) != std::string::npos)
		{
			if (modes[modes.find(charset[i]) - 1] == '+')
				_modes |= flag;
			if (modes[modes.find(charset[i]) - 1] == '-')
				_modes &= ~flag;
		}
		flag <<= 1;
	}
	return (modes);
}

std::string		Client::getModes() const
{
	std::string modesString("+");

	if (hasModes(CLIENT_FLAG_I) == true)
		modesString += "i";
	if (hasModes(CLIENT_FLAG_O) == true)
		modesString += "o";
	return modesString;
}

bool			Client::hasModes(mode_t modes) const
{
	return ((_modes | modes) == _modes);
}

int				Client::retryPassword()
{
	return --_retriesLeft;
}

//~~ CHANNELS

void	Client::joinChannel(Channel* channel)	
{
	_channels.insert(std::make_pair(channel->getName(), channel));
	if (channel->getClient(_nickname) == NULL)
		channel->addClient(this);
}

void	Client::leaveChannel(Channel* channel, Server* serv)
{
	_channels.erase(channel->getName());
	if (channel->getClient(_nickname) != NULL)
		channel->removeClient(this, serv);
	else if (channel->clientCount() == 0)
		serv->removeChannel(channel);
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
	return (!_channels.size() ? "*" : _channels.begin()->second->getName());
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
		else if (it->second->hasModes(CHANNEL_FLAG_M) && it->second->hasVoice(this))
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
