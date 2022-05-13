#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	// stats ?
}

Server::~Server()
{

}

std::string	Server::getPort()
{
	return (this->_port);
}

std::string	Server::getPassword()
{
	return (this->_password);
}