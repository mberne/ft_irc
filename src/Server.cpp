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

void	Server::addClient(int sock)
{
	Client *newClient = new Client(sock, "", "", "", "");
	clients.push_back(newClient);
}


// void	Server::removeClient(Client *src, std::vector<Client*>::iterator it_vector, std::map<std::string, Client*>::iterator it_map)
// {
// 	disconnectClients.insert(src);
// 	clients.erase(it_vector);
// 	clientsByName.erase(it_map);
// }
