#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), sock(-1), online(false)
{
	// stats ?
}

Server::~Server()
{

}

int	Server::getPort() const
{
	return (this->_port);
}

std::string	Server::getPassword() const
{
	return (this->_password);
}

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock, "", "", "", "");
	
	clientsBySock.insert(std::pair<int, Client *>(sock, newClient));
}


void	Server::removeClient(Client *src)
{
	oldClients.insert(std::pair<std::string, Client *>(src->getNickname(), src));
	clientsByName.erase(src->getNickname());
	clientsBySock.erase(src->getSock());
}
