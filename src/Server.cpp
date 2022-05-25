#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	online = 0;
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

// void	Server::addClient(int sock)
// {
// 	Client *newClient = new Client(sock, "", "", "", "");
// 	clientsBySocket.insert(sock, newClient);
// }

// void	Server::removeClient(Client *src, int it)
// {
// 	disconnectClients.insert(src->getNickname(), src);
// 	clientsByName.erase(it);
// 	clientsBySocket.erase(it);
// }
