#include "Channel.hpp"

//~~ CONSTRUCTOR

BanMask::BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos) :
				_mask(banMask), \
				_nick(banMask.substr(0, firstSepPos)), \
				_ident(banMask.substr(firstSepPos + 1, secondSepPos - firstSepPos - 1)), \
				_host(banMask.substr(secondSepPos + 1, banMask.size() - secondSepPos - 1)) {}

Channel::Channel(std::string name) : _name(name), _userLimit(CLIENT_LIMIT_PER_CHANNEL), _mods(0) {}

//~~ DESTRUCTOR

BanMask::~BanMask() {}

Channel::~Channel() {}

//~~ CHANNEL INFO

std::string		Channel::getName() const
{
	return _name;
}

std::string		Channel::getTopic() const
{
	return _topic;
}

void			Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string		Channel::getPassword() const
{
	return _password;
}

int				Channel::clientCount() const
{
	return _clients.size();
}

int				Channel::getUserLimit() const
{
	return _userLimit;
}

//~~ BanMask METHODS

bool		BanMask::isClientBanned(Client* client) const
{
	return (BanMask::stringCorrespondToMask(client->getNickname(), _nick) == true \
		&& BanMask::stringCorrespondToMask(client->getUser(), _ident) == true \
		&& BanMask::stringCorrespondToMask(client->getHost(), _host) == true);
}

bool		BanMask::stringCorrespondToMask(std::string str, std::string mask) // NEED TEST!
{
	size_t	i = 0;
	size_t	j = 0;

	while (i < str.size() && j < mask.size())
	{
		if (mask[j] == '*')
		{
			size_t		start, end;
			std::string	needle;

			start = mask.find_first_not_of("*", j);
			if (start == std::string::npos)
				return true;

			end = mask.find_first_of("*", start);
			if (end == std::string::npos)
				end = mask.size();
			
			needle = mask.substr(start, end - start);
			i = str.find(needle, i);
			j = end;

			if (i == std::string::npos)
				return false;
			i += needle.size();
		}
		else if (str[i] == mask[j] && i < str.size())
		{
			i++;
			j++;
		}
		else
			return false;
	}
	j = mask.find_first_not_of("*", j);
	if (j == std::string::npos)
		j = mask.size();
	return (i == str.size() && j == mask.size());
}

//~~ MODS

std::string		Channel::setMods(std::string mods, std::map<char, std::string>& modsArgs)
{
	std::string		charset("psitnm");
	std::string		args;
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
	if (mods.find('o') != std::string::npos)
	{
		if (mods[mods.find('o') - 1] == '+')
			addOperator(getClient(modsArgs.find('o')->second));
		if (mods[mods.find('o') - 1] == '-')
			removeOperator(getClient(modsArgs.find('o')->second));
		args += (" " + modsArgs.find('o')->second);
	}
	if (mods.find('l') != std::string::npos)
	{
		if (mods[mods.find('l') - 1] == '+')
			_userLimit = (std::atoi(modsArgs.find('l')->second.c_str()) >= CLIENT_LIMIT_PER_CHANNEL ? CLIENT_LIMIT_PER_CHANNEL : std::atoi(modsArgs.find('l')->second.c_str()));
		if (mods[mods.find('l') - 1] == '-')
			_userLimit = CLIENT_LIMIT_PER_CHANNEL;
		args += (" " + modsArgs.find('l')->second);
	}
	if (mods.find('b') != std::string::npos)
	{
		if (mods[mods.find('b') - 1] == '+')
			addBanMask(modsArgs.find('b')->second);
		if (mods[mods.find('b') - 1] == '-')
			removeBanMask(modsArgs.find('b')->second);
		args += (" " + modsArgs.find('b')->second);
	}
	if (mods.find('v') != std::string::npos)
	{
		if (mods[mods.find('v') - 1] == '+')
			addClientWithVoice(getClient(modsArgs.find('v')->second));
		if (mods[mods.find('v') - 1] == '-')
			removeClientWithVoice(getClient(modsArgs.find('v')->second));
		args += (" " + modsArgs.find('v')->second);
	}
	if (mods.find('k') != std::string::npos)
	{
		if (mods[mods.find('k') - 1] == '+')
			_password = modsArgs.find('k')->second;
		if (mods[mods.find('k') - 1] == '-' && _password.compare(modsArgs.find('k')->second) == 0)
		{
			_password = "";
			modsArgs.find('k')->second = "*";
		}
		args += (" " + modsArgs.find('k')->second);
	}
	return (mods + args);
}


std::string		Channel::getMods() const
{
	std::string modsString("+");

	if ((_mods | CHANNEL_FLAG_P) == _mods)
		modsString.push_back('p');
	if ((_mods | CHANNEL_FLAG_S) == _mods)
		modsString.push_back('s');
	if ((_mods | CHANNEL_FLAG_I) == _mods)
		modsString.push_back('i');
	if ((_mods | CHANNEL_FLAG_T) == _mods)
		modsString.push_back('t');
	if ((_mods | CHANNEL_FLAG_N) == _mods)
		modsString.push_back('n');
	if ((_mods | CHANNEL_FLAG_M) == _mods)
		modsString.push_back('m');
	if (_userLimit < CLIENT_LIMIT_PER_CHANNEL)
		modsString.push_back('l');
	if (!_password.empty())
		modsString.push_back('k');
	if (_userLimit < CLIENT_LIMIT_PER_CHANNEL)
		modsString += (" " + std::to_string(_userLimit));
	if (!_password.empty())
		modsString += (" " + _password);
	return modsString;
}

bool			Channel::hasMod(int mode) const
{
	return ((_mods | mode) == _mods);
}

void			Channel::addOperator(Client* client)
{
	if (_operators.find(client->getNickname()) == _operators.end())
		_operators.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeOperator(Client* client)
{
	if (_operators.find(client->getNickname()) != _operators.end())
		_operators.erase(client->getNickname());
}

void			Channel::addClientWithVoice(Client* client)
{
	if (_clientsWithVoicePerm.find(client->getNickname()) == _clientsWithVoicePerm.end())
		_clientsWithVoicePerm.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeClientWithVoice(Client* client)
{
	if (_clientsWithVoicePerm.find(client->getNickname()) != _clientsWithVoicePerm.end())
		_clientsWithVoicePerm.erase(client->getNickname());
}

void			Channel::addBanMask(std::string banMask)
{
	if (_banList.find(banMask) == _banList.end())
		_banList.insert(std::make_pair(banMask, BanMask(banMask, banMask.find_first_of('!'), banMask.find_first_of('@'))));
}

void			Channel::removeBanMask(std::string banMask)
{
	if (_banList.find(banMask) != _banList.end())
		_banList.erase(banMask);
}

//~~ CLIENTS

void			Channel::addClient(Client* client)
{
	_clients.insert(std::make_pair(client->getNickname(), client));
	if (client->getChannel(_name) == NULL)
		client->joinChannel(this);
}

void			Channel::removeClient(Client* client, Server* serv)
{
	_clients.erase(client->getNickname());
	removeClientWithVoice(client);
	removeOperator(client);
	if (client->getChannel(_name) != NULL)
		client->leaveChannel(this, serv);
	else if (clientCount() == 0)
		serv->removeChannel(this);
}

Client*			Channel::getClient(std::string name) const
{
	if (_clients.find(name) == _clients.end())
		return (NULL);
	else
		return (_clients.find(name)->second);
}

bool			Channel::isOperator(Client* client) const
{
	return (_operators.find(client->getNickname()) != _operators.end());
}

bool			Channel::hasVoice(Client* client) const
{
	return (_clientsWithVoicePerm.find(client->getNickname()) != _clientsWithVoicePerm.end());
}

bool			Channel::isBanned(Client* client)
{
	for (std::map<std::string, BanMask>::iterator it = _banList.begin(); it != _banList.end(); it++)
		if (it->second.isClientBanned(client) == true)
			return true;
	return false;
}

bool			Channel::isInvited(Client* client) const
{
	return ((_mods | CHANNEL_FLAG_I) != _mods || _invitedClients.find(client->getNickname()) != _invitedClients.end());
}

std::string		Channel::showClientsList()
{
	std::string		clientList;

	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (!it->second->hasMod(CLIENT_FLAG_I))
		{
			if (it != _clients.begin())
				clientList += " ";
			if (this->isOperator(it->second))
				clientList += "@";
			else if (this->hasMod(CHANNEL_FLAG_M) && this->hasVoice(it->second))
				clientList += "+";
			clientList += it->second->getNickname();
		}
	}
	return clientList;
}

void			Channel::sendToClients(std::string msg, Client* sender)
{
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second != sender)
			it->second->addToOutputBuffer(msg);
	}
}

std::map<std::string, Client*> &	Channel::getAllClients()
{
	return _clients;
}
