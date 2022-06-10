#include "Channel.hpp"

//~~ CONSTRUCTOR

BanMask::BanMask(std::string banMask, size_t firstSepPos, size_t secondSepPos) :
				_mask(banMask), \
				_nick(banMask.substr(0, firstSepPos)), \
				_ident(banMask.substr(firstSepPos + 1, secondSepPos - firstSepPos - 1)), \
				_host(banMask.substr(secondSepPos + 1, banMask.size() - secondSepPos - 1)) {}

Channel::Channel(std::string name) : _name(name), _userLimit(-1), _mods(0) {}

//~~ DESTRUCTOR

BanMask::~BanMask() {}

Channel::~Channel() {}

//~~ CHANNEL INFO

std::string		Channel::getName() const
{
	return (_name);
}

std::string		Channel::getTopic() const
{
	return (_topic);
}

std::string		Channel::getPassword() const
{
	return (_password);
}

int				Channel::clientCount() const
{
	return (_clients.size());
}

int				Channel::getUserLimit() const
{
	return (_userLimit);
}

bool			Channel::isModerated() const
{
	return ((_mods | CHANNEL_FLAG_M) == _mods);
}

bool			Channel::isInvite() const
{
	return ((_mods | CHANNEL_FLAG_I) == _mods);
}

bool			Channel::nonMembersCanTalk() const
{
	return ((_mods | CHANNEL_FLAG_N) == _mods);
}

bool			Channel::isPrivate() const
{
	return ((_mods | CHANNEL_FLAG_P) == _mods);
}

bool			Channel::isSecret() const
{
	return ((_mods | CHANNEL_FLAG_S) == _mods);
}

bool			Channel::hasTopic() const
{
	return ((_mods | CHANNEL_FLAG_T) == _mods);
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

//~~ MODS

std::string		Channel::getMods() const
{
	std::string modsString("+");

	if (isPrivate() == true)
		modsString += 'p';
	if (isSecret() == true)
		modsString += 's';
	if (isInvite() == true)
		modsString += 'i';
	if (hasTopic() == true)
		modsString += 't';
	if (nonMembersCanTalk() == true)
		modsString += 'n';
	if (isModerated() == true)
		modsString += 'm';
	return (modsString);
}

void			Channel::addOperator(Client* client)
{
	_operators.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeOperator(Client* client)
{
	_operators.erase(client->getNickname());
}

void			Channel::addclientWithVoice(Client* client)
{
	_clientsWithVoicePerm.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeclientWithVoice(Client* client)
{
	_clientsWithVoicePerm.erase(client->getNickname());
}

void			Channel::addBanMask(std::string banMask)
{
	_banList.insert(std::make_pair(banMask, BanMask(banMask, banMask.find_first_of('!'), banMask.find_first_of('@'))));
}

void			Channel::removeBanMask(std::string banMask)
{
	_banList.erase(banMask);
}

//~~ CLIENTS

void			Channel::addClient(Client* client)
{
	// Error handling needed!
	_clients.insert(std::make_pair(client->getNickname(), client));
}

void			Channel::removeClient(Client* client)
{
	_clients.erase(client->getNickname());
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
