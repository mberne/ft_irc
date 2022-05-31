#include "irc.hpp"

int	sendMessages(t_env *irc, int numberSockets)
{
	size_t	i = 1;
	int		ret;

	while (i < irc->fds.size() && numberSockets > 0)
	{
		if (irc->fds[i].revents != 0) // request on this socket
		{
			ret = send(irc->fds[i].fd, "Response.\n", 10, MSG_DONTWAIT); // test
			if (ret == -1)
			{
				if (errno == ECONNRESET) // deconnexion
				{
					irc->fds.erase(irc->fds.begin() + i);
					// TODO: create the client in disconnectClients map and remove this client in clients map --> irc->serv->removeClient(-, irc->fds.begin() + i);
				}
				else if (errno != EWOULDBLOCK && errno != EAGAIN)
					return EXIT_FAILURE;
			}
		}
		i++;
		numberSockets--;
	}
	return EXIT_SUCCESS;
}

int	receiveMessages(t_env *irc, int numberSockets)
{
	int 	ret;
	size_t	i = 1;
	void	*buf[MAX_MESSAGE_LENGTH];

	while (i < irc->fds.size() && numberSockets > 0)
	{
		if (irc->fds[i].revents != 0) // request on this socket
		{
			if (irc->fds[i].revents == POLLHUP) // deconnexion
			{
				irc->fds.erase(irc->fds.begin() + i);
				// irc->serv->removeClient(irc->serv->clients[i], irc->serv->clients.begin() + i, irc->serv->clientsByName.begin() + i);
			}
			else if (irc->fds[i].revents == POLLIN || irc->fds[i].revents == POLLOUT)
			{
				ret = recv(irc->fds[i].fd, buf, MAX_MESSAGE_LENGTH, MSG_DONTWAIT);
				if (ret == 0)
				{
					irc->fds.erase(irc->fds.begin() + i); // deconnexion
					// TODO: create the client in disconnectClients map and remove this client in clients map --> irc->serv->removeClient(-, irc->fds.begin() + i);
				}
				else if (ret > 0)
				{
					std::cout << "Message from : " << i << std::endl; // test
					// TODO: pthomas(buf, ret); parsing, exec and keep write in buffers
				}
				else if (ret < 0 && errno != EWOULDBLOCK && errno != EAGAIN)
					return EXIT_FAILURE;
			}
			else
				return EXIT_FAILURE;
		}
		i++;
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
		else if (ret > 0 && irc->fds.size() < RLIMIT_NOFILE - 1)
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
	int 			numberSockets;
	struct pollfd	tmp;

	tmp.fd = irc->serv->sock; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	irc->fds.push_back(tmp);
	irc->serv->online = 1;
	while (irc->serv->online)
	{
		numberSockets = poll(&irc->fds[0], irc->fds.size(), -1); // return the number of socket with request, -1 = attente infinie OU 0 = pas d'attente
		if (numberSockets == -1)
			return EXIT_FAILURE;
		if (acceptConnexions(irc) == EXIT_FAILURE) // the server accept connexions
			return EXIT_FAILURE;
		if (receiveMessages(irc, numberSockets) == EXIT_FAILURE) // the server retrieves the requests
			return EXIT_FAILURE;
		if (sendMessages(irc, numberSockets) == EXIT_FAILURE) // the server responds to requests
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
