#include "Channel.hpp"

//~~ CONSTRUCTOR

BanMask::BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos) :
				_mask(banMask), \
				_nick(banMask.substr(0, firstSepPos)), \
				_ident(banMask.substr(firstSepPos + 1, secondSepPos - firstSepPos - 1)), \
				_host(banMask.substr(secondSepPos + 1, banMask.size() - secondSepPos - 1)) {}

Channel::Channel(std::string name) : _name(name), _userLimit(-1), _mods(0) {}

//~~ DESTRUCTOR

BanMask::~BanMask(void) {}

Channel::~Channel(void) {}

//~~ ACCESSOR

std::string		Channel::getName(void) const
{
	return (_name);
}

std::string		Channel::getTopic(void) const
{
	return (_topic);
}

void			Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string		Channel::getPassword(void) const
{
	return (_password);
}

void			Channel::setPassword(std::string password)
{
	_password = password;
}

int				Channel::getUserLimit(void) const
{
	return (_userLimit);
}

void			Channel::setUserLimit(int userLimit)
{
	_userLimit = userLimit;
}

int				Channel::getMods(void) const
{
	return (_mods);
}

void			Channel::setMods(int mods)
{
	_mods = mods;
}

//~~ METHODS

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
				return (true);

			end = mask.find_first_of("*", start);
			if (end == std::string::npos)
				end = mask.size();
			
			needle = mask.substr(start, end - start);
			i = str.find(needle, i);
			j = end;

			if (i == std::string::npos)
				return (false);
			i += needle.size();
		}
		else if (str[i] == mask[j] && i < str.size())
		{
			i++;
			j++;
		}
		else
			return (false);
	}
	j = mask.find_first_not_of("*", j);
	if (j == std::string::npos)
		j = mask.size();
	return (i == str.size() && j == mask.size());
}

void			Channel::addClient(Client* client)
{
	// Error handling needed!
	_clients.insert(std::pair<std::string, Client*>(client->getNickname(), client));
}

void			Channel::removeClient(Client* client)
{
	_clients.erase(client->getNickname());
}

void			Channel::addOperator(Client* client)
{
	_operators.insert(std::pair<std::string, Client*>(client->getNickname(), client));
}

void			Channel::removeOperator(Client* client)
{
	_operators.erase(client->getNickname());
}

void			Channel::addclientWithVoice(Client* client)
{
	_clientsWithVoicePerm.insert(std::pair<std::string, Client*>(client->getNickname(), client));
}

void			Channel::removeclientWithVoice(Client* client)
{
	_clientsWithVoicePerm.erase(client->getNickname());
}

void			Channel::addBanMask(std::string banMask)
{
	_banList.insert(std::pair<std::string, BanMask>(banMask, BanMask(banMask, banMask.find_first_of('!'), banMask.find_first_of('@'))));
}

void			Channel::removeBanMask(std::string banMask)
{
	_banList.erase(banMask);
}

bool			Channel::isConnected(Client* client) const
{
	return (_clients.find(client->getNickname()) != _clients.end());
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
			return (true);
	return (false);
}

// t'es beau <3
