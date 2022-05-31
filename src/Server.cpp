#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	sock = -1;	// Même si tu ne les initialise pas avec des paramètres, il doivent se trouver dans la liste d'initialisation. Exemple : [...]: _port(port), _password(password), sock(-1), online(false)
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
	Client *newClient = new Client(sock, "", "", "", ""); // Sujet à changer quand j'aurais update mes classes
	clients.push_back(newClient); // Tu peux directement pushback le retour de new sans le mettre dans une variable. Exemple : clients.push_back(new Client(sock, "", "", "", ""));
}


// void	Server::removeClient(Client *src, std::vector<Client*>::iterator it_vector, std::map<std::string, Client*>::iterator it_map)
// {
// 	disconnectClients.insert(src);
// 	clients.erase(it_vector);
// 	clientsByName.erase(it_map);
// }
