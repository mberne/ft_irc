#include "Channel.hpp"

//~~ CONSTRUCTOR

BanMask::BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos) :
				_mask(banMask), \
				_nick(banMask.substr(0, firstSepPos)), \
				_ident(banMask.substr(firstSepPos + 1, secondSepPos - firstSepPos - 1)), \
				_host(banMask.substr(secondSepPos + 1, banMask.size() - secondSepPos - 1)) {}

Channel::Channel(std::string name) : _name(name), _userLimit(CHANNEL_CLIENTS_LIMIT), _modes(0) {}

//~~ DESTRUCTOR

BanMask::~BanMask() {}

Channel::~Channel() {}

//~~ BanMask METHODS

bool		BanMask::isClientBanned(Client* client) const
{
	return (BanMask::stringCorrespondToMask(client->getNickname(), _nick) == true \
		&& BanMask::stringCorrespondToMask(client->getUser(), _ident) == true \
		&& BanMask::stringCorrespondToMask(client->getHost(), _host) == true);
}

std::string		BanMask::getBanMask() const
{
	return _mask;
}

bool		BanMask::stringCorrespondToMask(std::string str, std::string mask)
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

std::string	Channel::getTopicTime() const
{
	return std::to_string(_topicTime);
}

void	Channel::setTopicTime()
{
	_topicTime = time(NULL);
}

std::string	Channel::getTopicWriter() const
{
	return _topicWriter;
}

void	Channel::setTopicWriter(std::string writer)
{
	_topicWriter = writer;
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

std::map<std::string, BanMask>&	Channel::getBanList()
{
	return _banList;
}

//~~ MODES

void	Channel::addModes(char mode, std::map<char, std::string>& modesArgs, bool& change)
{
	int newLimit;
	switch (mode)
	{
		case 'o':
			if (isOperator(getClient(modesArgs.find('o')->second)) == false)
				change = 1;
			addOperator(getClient(modesArgs.find('o')->second));
			break;
		case 'l':
			newLimit = std::stoi(modesArgs.find('l')->second) >= CHANNEL_CLIENTS_LIMIT ? CHANNEL_CLIENTS_LIMIT : std::atoi(modesArgs.find('l')->second.c_str());
			if (_userLimit != newLimit)
				change = 1;
			_userLimit = newLimit;
			break;
		case 'b':
			if (_banList.find(modesArgs.find('b')->second) == _banList.end())
				change = 1;
			addBanMask(modesArgs.find('b')->second);
			break;
		case 'v':
			if (hasVoice(getClient(modesArgs.find('v')->second)) == false)
				change = 1;
			addClientWithVoice(getClient(modesArgs.find('v')->second));
			break;
		case 'k':
			if (_password.compare(modesArgs.find('k')->second) != 0)
				change = 1;
			_password = modesArgs.find('k')->second;
			break;
	}
}

void	Channel::removeModes(char mode, std::map<char, std::string>& modesArgs, bool& change)
{
	switch (mode)
	{
		case 'o':
			if (isOperator(getClient(modesArgs.find('o')->second)) == true)
				change = 1;
			removeOperator(getClient(modesArgs.find('o')->second));
			break;
		case 'l':
			if (_userLimit != CHANNEL_CLIENTS_LIMIT)
				change = 1;
			_userLimit = CHANNEL_CLIENTS_LIMIT;
			break;
		case 'b':
			if (_banList.find(modesArgs.find('b')->second) != _banList.end())
				change = 1;
			removeBanMask(modesArgs.find('b')->second);
			break;
		case 'v':
			if (hasVoice(getClient(modesArgs.find('v')->second)) == true)
				change = 1;
			removeClientWithVoice(getClient(modesArgs.find('v')->second));
			break;
		case 'k':
			if (_password.compare(modesArgs.find('k')->second) == 0)
			{
				change = 1;
				_password = "";
				modesArgs.find('k')->second = "*";
			}
			break;
	}
}

std::string		Channel::setModes(std::string validModes, std::map<char, std::string>& modesArgs)
{
	std::string		modString;
	std::string		args;

	std::string		charset("psitnm");
	mode_t			flag = 1;

	for (size_t	i = 0; i < charset.size(); i++)
	{
		if (validModes.find(charset[i]) != std::string::npos)
		{
			char sign = validModes[validModes.find(charset[i]) - 1];
			bool change = false;

			if (sign == '+')
			{
				if ((_modes | flag) != _modes)
					change = true;
				_modes |= flag;
			}
			else if (sign == '-')
			{
				if ((_modes & ~flag) != _modes)
					change = true;
				_modes &= ~flag;
			}

			if (change == true)
			{
				if (modString[modString.find_last_of("+-", charset[i])] != sign)
					modString.push_back(sign);
				modString.push_back(charset[i]);
			}
		}
		flag <<= 1;
	}

	charset = "olbvk";
	
	for (size_t	i = 0; i < charset.size(); i++)
	{
		if (validModes.find(charset[i]) != std::string::npos)
		{
			char sign = validModes[validModes.find(charset[i]) - 1];
			bool change = false;

			if (sign == '+')
				addModes(charset[i], modesArgs, change);
			else if (sign == '-')
				removeModes(charset[i], modesArgs, change);
			if (change == true)
			{
				if (modString[modString.find_last_of("+-", charset[i])] != sign)
					modString.push_back(sign);
				modString.push_back(charset[i]);
				if (modesArgs.find(charset[i]) != modesArgs.end())
					args += (" " + modesArgs.find(charset[i])->second);
			}
		}
	}
	return (modString + args);
}


std::string		Channel::getModes() const
{
	std::string modesString("+");

	if ((_modes | CHANNEL_FLAG_P) == _modes)
		modesString.push_back('p');
	if ((_modes | CHANNEL_FLAG_S) == _modes)
		modesString.push_back('s');
	if ((_modes | CHANNEL_FLAG_I) == _modes)
		modesString.push_back('i');
	if ((_modes | CHANNEL_FLAG_T) == _modes)
		modesString.push_back('t');
	if ((_modes | CHANNEL_FLAG_N) == _modes)
		modesString.push_back('n');
	if ((_modes | CHANNEL_FLAG_M) == _modes)
		modesString.push_back('m');
	if (_userLimit < CHANNEL_CLIENTS_LIMIT)
		modesString.push_back('l');
	if (!_password.empty())
		modesString.push_back('k');
	if (_userLimit < CHANNEL_CLIENTS_LIMIT)
		modesString += (" " + std::to_string(_userLimit));
	if (!_password.empty())
		modesString += (" " + _password);
	return modesString;
}

bool			Channel::hasModes(mode_t modes) const
{
	return ((_modes | modes) == _modes);
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

void			Channel::addInvitedClient(Client* client)
{
	if (_invitedClients.find(client->getNickname()) == _invitedClients.end())
		_invitedClients.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeInvitedClient(Client* client)
{
	if (_invitedClients.find(client->getNickname()) != _invitedClients.end())
		_invitedClients.erase(client->getNickname());
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
	removeInvitedClient(client);
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
	return (_invitedClients.find(client->getNickname()) != _invitedClients.end());
}

bool	Channel::ableToTalk(Client *client)
{
	if (isBanned(client) || \
		(hasModes(CHANNEL_FLAG_N) && !getClient(client->getNickname())) || \
		(hasModes(CHANNEL_FLAG_M) && !isOperator(client) && !hasVoice(client)))
		return false;
	return true;
}

std::string		Channel::showClientsList()
{
	std::string		clientList;

	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (!it->second->hasModes(CLIENT_FLAG_I))
		{
			if (it != _clients.begin())
				clientList += " ";
			if (this->isOperator(it->second))
				clientList += "@";
			else if (this->hasModes(CHANNEL_FLAG_M) && this->hasVoice(it->second))
				clientList += "+";
			clientList += it->second->getNickname();
		}
	}
	return clientList;
}

void			Channel::sendToClients(std::string msg, Client* sender)
{
	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->second != sender)
			it->second->addToOutputBuffer(msg);
}

std::map<std::string, Client*> &	Channel::getAllClients()
{
	return _clients;
}
