
#include "IRC.hpp"

static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << "\n > Closing and cleaning ..." << std::endl;
}

void irc(Server *server)
{
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	char 				buff[256]; //	FOR MESSAGE RECEIVING/SENDING

	server->initSocket();
	server->initBind(&server_addr);
	listen(server->getBaseSocket(), 16);

/*
//	EXAMPLE CODE FOR MAIN WHILE LOOP GOAL

	int					socket_count = 0;
	fd_set				socket_master;

	while (!stopFlag) --> ok
	{
		fd_set	socket_set = socket_master;

		socket_count = select(0, &socket_set, nullptr, nullptr, nullptr);
		if (socket_count <= 0)
			continue;
		for (int i = 0; i < socket_count; i++)
		{
			if (!stopFlag)
				break;

			new_socket_fd = socket_set.fd_array[i];

			if (new_socket_fd == base_socket_fd)
				read ?
			else
				send ?
		}
	}
	close (server->getBaseSocket());
	close (server->getNewSocket());
*/

//		Accepts an incoming connection request
		new_socket_fd = accept(base_socket_fd, (struct sockaddr *) &client_addr, &client_len);
		if (new_socket_fd < -1)
			throw std::invalid_argument(" > Error at accept(): ");
		else if (new_socket_fd != -1)
		{
			std::cout << "\nCONNECTED\nCLIENT FROM NETWORK :\t" << inet_ntoa(client_addr.sin_addr) << "\nNET-TO-HOST PORT :\t" << ntohs(client_addr.sin_port) << std::endl;
			std::cout << "\nSERVER IP (local): \t" << inet_ntoa(server_addr.sin_addr) << "\nHOST-TO-NET PORT :\t" << ntohs(server_addr.sin_port) << std::endl;
		}

//		Receives an incoming message (in theory at least)
		bzero(buff, 256);
		if (new_socket_fd >= 0 && recv(new_socket_fd, buff, 255, 0) >= 0)
		{
			std::cout << std::endl << "Received Message : " << buff << std::endl;
		}

		bzero(buff, 256);
//		Receives an outgoing message (in theory at least)
		if (new_socket_fd >= 0 && send(new_socket_fd, "Hello, client!\n", 14, 0) >= 0)
		{
			std::cout << std::endl << "Sent Message : " << "Hello, client!\n" << std::endl;
		}
	}
	close (server->getBaseSocket());
	close (server->getNewSocket());

}
int	main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN); //	resets signal
	signal(SIGINT, stop);

	try
	{
		int port = atoi(av[1]);
		int pass = atoi(av[2]);
		(void) pass;
		Server server(port);
		if (ac != 3)
			throw std::invalid_argument(" > Error main(): Invalid argument count.");
//		Arg parsing
		Server test;
		test.irc(atoi(av[1]), atoi(av[2]));
		irc(&server);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		if (errno)
			std::cout << std::strerror(errno) << std::endl;
		// close (server.getBaseSocket());
		// close (server.getNewSocket());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}