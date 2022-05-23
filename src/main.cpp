#include "irc.hpp"
#include "Server.hpp"

int	serverSend(t_env *irc, int numberSockets)
{
	size_t	i = 1;
	int		ret;

	while (i < irc->fds.size() && numberSockets > 0) // send
	{
		// ret = send(irc->fds[i].fd, msg, msgLength, 0);
		ret = 1;
		if (ret == -1)
		{
			if (errno == ECONNRESET)
				irc->fds.erase(irc->fds.begin() + i + 1);
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
		ret = accept(irc->fds[0].fd, (struct sockaddr*)&irc->servSocket, NULL);
		// static_cast<struct sockaddr*>(&irc->servSocket) ??
		if (errno == EWOULDBLOCK)
			break ;
		else if (ret > 0)
		{
			tmp.fd = ret;
			tmp.events = POLLIN;
			irc->fds.push_back(tmp);
		}
		else
			return EXIT_FAILURE;
	} while (ret > 0);
	while (i < irc->fds.size() && numberSockets > 0) // recv
	{
		if (irc->fds[i].revents != 0) // there is an event on this socket
		{
			if (irc->fds[i].revents == POLLHUP) // deconnexion
				irc->fds.erase(irc->fds.begin() + i + 1);
			else if (irc->fds[i].revents == POLLIN || irc->fds[i].revents == POLLOUT)
			{
				ret = recv(irc->fds[i].fd, buf, MAX_MESSAGE_LENGTH, 0);
				if (ret == 0)
					irc->fds.erase(irc->fds.begin() + i + 1); // rip si on n'a rien a recevoir ?
				else if (ret > 0)
				{
					std::cout << "Yo" << std::endl;
					// pthomas(buf, ret); parsing, exec and keep write in buffers
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

	tmp.fd = irc->serv->sock; // Initialise the first listening socket
	tmp.events = POLLIN;
	irc->fds.push_back(tmp);
	while (irc->serv)
	{
		numberSockets = poll(&irc->fds[0], irc->fds.size(), 0); // récupère le nombre de socket/client qui a une requête, NULL = non bloquant
		if (numberSockets == -1)
			return EXIT_FAILURE;
		if (serverRecv(irc, numberSockets) == EXIT_FAILURE)
			return EXIT_FAILURE;
		if (serverSend(irc, numberSockets) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	createServSocket(t_env *irc)
{
	irc->serv->sock = socket(PF_INET, SOCK_STREAM, irc->pe->p_proto); // settings TCP
	if (irc->serv->sock == -1)
		return EXIT_FAILURE;
	irc->servSocket.sin_family = PF_INET; // address format IPV6
	irc->servSocket.sin_port = htons(irc->serv->getPort()); // convert port
	irc->servSocket.sin_addr.s_addr = htonl(INADDR_ANY); // any sources accepted
	if (bind(irc->serv->sock, (struct sockaddr*)&irc->servSocket, sizeof(irc->servSocket)) == -1)
		return EXIT_FAILURE;
	// static_cast<struct sockaddr*>(&irc->servSocket) ??
	if (listen(irc->serv->sock, 0) == -1)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int initServ(int ac, char **av, t_env *irc)
{
	if (ac != 3)
	{
		std::cerr << USAGE << std::endl;
		return EXIT_FAILURE;
	}
	int tmp = std::atoi(av[1]);
	if (tmp < 0 || tmp > INT32_MAX || !av[2][0])
	{
		errno = EINVAL;
		perror(SERV_NAME); 
		return EXIT_FAILURE;
	}
	irc->serv = new Server(tmp, av[2]);
	if (!irc->serv)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
	t_env	irc;

	if (initServ(ac, av, &irc) == EXIT_FAILURE)
		return EXIT_FAILURE;
	if ((irc.pe = getprotobyname(PROTOCOL)) == NULL)
		return EXIT_FAILURE; // + free serv
	if (createServSocket(&irc) == EXIT_FAILURE)
		return EXIT_FAILURE; // + free serv
	if (mainLoop(&irc) == EXIT_FAILURE)
		return EXIT_FAILURE; // + free serv, fd
	// free irc.serv, viderle vector de pollfd
	return EXIT_SUCCESS;
}
