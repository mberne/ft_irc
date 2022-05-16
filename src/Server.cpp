#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
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
