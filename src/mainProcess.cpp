#include "irc.hpp"

int	serverSend(t_env *irc, int numberSockets)
{
	size_t	i = 1;
	int		ret;

	while (i < irc->fds.size() && numberSockets > 0)
	{
		ret = send(irc->fds[i].fd, "Yo mec.", 8, 0);
		if (ret == -1)
		{
			if (errno == ECONNRESET) // deconnexion
				irc->fds.erase(irc->fds.begin() + i + 1);
				// TODO: create the client in disconnectClients map and remove this client in clients map
			else
				return EXIT_FAILURE;
		}
		i++;
		numberSockets--;
	}
	return EXIT_SUCCESS;
}

int	serverRecv(t_env *irc, int numberSockets)
{
	int 			ret;
	struct pollfd	tmp;
	size_t			i = 1;
	void			*buf[MAX_MESSAGE_LENGTH];

	do // accept all connexions
	{
		socklen_t addrlen = sizeof(irc->servSocket);
		ret = accept(irc->fds[0].fd, (struct sockaddr*)&irc->servSocket, &addrlen);
		// TOFIX: static_cast<struct sockaddr*>(&irc->servSocket) ??
		if (errno == EWOULDBLOCK)
			break ;
		else if (ret > 0)
		{
			std::cout << "Bonjour." << std::endl;
			tmp.fd = ret;
			tmp.events = POLLIN;
			if (fcntl(tmp.fd, F_SETFL, O_NONBLOCK)) // client fd non blocking
				return EXIT_FAILURE;
			irc->fds.push_back(tmp);
			// TODO: create a client in clients map
		}
		else
			return EXIT_FAILURE;
	} while (ret > 0);
	while (i < irc->fds.size() && numberSockets > 0) // retrieves requests
	{
		if (irc->fds[i].revents != 0) // request on this socket
		{
			if (irc->fds[i].revents == POLLHUP) // deconnexion
				irc->fds.erase(irc->fds.begin() + i + 1);
				// TODO: create the client in disconnectClients map and remove this client in clients map
			else if (irc->fds[i].revents == POLLIN || irc->fds[i].revents == POLLOUT)
			{
				ret = recv(irc->fds[i].fd, buf, MAX_MESSAGE_LENGTH, 0);
				if (ret == 0)
					irc->fds.erase(irc->fds.begin() + i + 1); // deconnexion | rip si on n'a rien a recevoir ?
					// TODO: create the client in disconnectClients map and remove this client in clients map
				else if (ret > 0)
				{
					std::cout << "Yo la team." << std::endl;
					// TODO: pthomas(buf, ret); parsing, exec and keep write in buffers
				}
				else
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

int	mainLoop(t_env *irc)
{
	int 			numberSockets;
	struct pollfd	tmp;

	tmp.fd = irc->serv->sock; // initialize the first socket in pollfd (the server here)
	tmp.events = POLLIN;
	irc->fds.push_back(tmp);
	while (1) // TOFIX: serv online instead
	{
		numberSockets = poll(&irc->fds[0], irc->fds.size(), 0); // return the number of socket with request, NULL = non-blocking
		if (numberSockets == -1)
			return EXIT_FAILURE;
		if (serverRecv(irc, numberSockets) == EXIT_FAILURE) // the server accept connexions and retrieves the requests
			return EXIT_FAILURE;
		if (serverSend(irc, numberSockets) == EXIT_FAILURE) // the server responds to requests
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
