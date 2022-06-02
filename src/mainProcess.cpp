#include "ircserv.hpp"

int	sendMessages(t_env *irc, int numberSockets)
{
	std::vector<struct pollfd>::iterator	it = irc->fds.begin() + 1;
	int										ret;

	while (it < irc->fds.end() && numberSockets > 0)
	{
		Client *client = irc->serv->clientsBySock.find(it->fd)->second;
		if (it->revents != 0) // request on this socket
		{
			ret = send(it->fd, "Response.\n", 10, 0); // test
			if (ret == -1)
			{
				if (errno == ECONNRESET) // deconnexion
				{
					irc->fds.erase(it);
					irc->serv->removeClient(client);
				}
				else if (errno != EWOULDBLOCK && errno != EAGAIN)
					return EXIT_FAILURE;
			}
		}
		it++;
		numberSockets--;
	}
	return EXIT_SUCCESS;
}

int	receiveMessages(t_env *irc, int numberSockets)
{
	std::vector<struct pollfd>::iterator	it = irc->fds.begin() + 1;
	int 									ret;
	void									*buf[MAX_MESSAGE_LENGTH];


	while (it < irc->fds.end() && numberSockets > 0)
	{
		Client *client = irc->serv->clientsBySock.find(it->fd)->second;
		if (it->revents != 0) // request on this socket
		{
			if (it->revents == POLLHUP) // deconnexion
			{
				irc->fds.erase(it);
				irc->serv->removeClient(client);
			}
			else if (it->revents == POLLIN || it->revents == POLLOUT)
			{
				ret = recv(it->fd, buf, MAX_MESSAGE_LENGTH, 0);
				if (ret == 0)
				{
					irc->fds.erase(it); // deconnexion
					irc->serv->removeClient(client);
				}
				else if (ret > 0)
				{
					std::cout << "Message from socket : " << it->fd << std::endl; // test
					// TODO: pthomas(buf, ret); parsing, exec and keep write in buffers
				}
				else if (ret < 0 && errno != EWOULDBLOCK && errno != EAGAIN)
					return EXIT_FAILURE;
			}
			else
				return EXIT_FAILURE;
		}
		it++;
		numberSockets--;
	}
	return EXIT_SUCCESS;
}

int acceptConnexions(t_env *irc)
{
	int 			ret;
	struct pollfd	tmp;

	do
	{
		socklen_t addrlen = sizeof(irc->servSocket);
		ret = accept(irc->fds[0].fd, reinterpret_cast<struct sockaddr*>(&irc->servSocket), &addrlen);
		if (ret < 0 && (errno == EWOULDBLOCK || errno == EAGAIN))
			break ;
		else if (ret > 0 && irc->fds.size() < OPEN_MAX - 3)
		{
			std::cout << "Someone is connecting." << std::endl; // test
			tmp.fd = ret;
			tmp.events = POLLIN;
			if (fcntl(tmp.fd, F_SETFL, O_NONBLOCK) == -1) // client socket non blocking
				return EXIT_FAILURE;
			irc->fds.push_back(tmp);
			irc->serv->addClient(ret);
		}
		else if (ret > 0)
			std::cout << "Too much clients." << std::endl; // test
		else
			return EXIT_FAILURE;
	} while (ret > 0);
	return EXIT_SUCCESS;
}

int	mainLoop(t_env *irc)
{
	int	numberSockets; 

	irc->serv->online = 1;
	while (irc->serv->online)
	{
		numberSockets = poll(&irc->fds[0], irc->fds.size(), -1); // return the number of socket with request, -1 = attente infinie OU 0 = pas d'attente ?
		if (numberSockets == -1)
			return EXIT_FAILURE; // no exit
		if (acceptConnexions(irc) == EXIT_FAILURE) // the server accept connexions
			return EXIT_FAILURE; // no exit
		if (receiveMessages(irc, numberSockets) == EXIT_FAILURE) // the server retrieves the requests
			return EXIT_FAILURE; // no exit
		if (sendMessages(irc, numberSockets) == EXIT_FAILURE) // the server responds to requests
			return EXIT_FAILURE; // no exit
	}
	return EXIT_SUCCESS;
}
