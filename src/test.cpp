
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

	while (!stopFlag) --> ok
	{
		if (conectionCount <= 0)
			continue;
		foreach (user)
		{
			if (stopFlag)
				break;
			info = poll(user);
			if (!info.stuff_to_do) //
				continue;
			process_request(user);
		}
	}
	close (server->getBaseSocket());
	close (server->getNewSocket());
*/

	// Client interaction loop
	while (!stopFlag)
	{
		server->acceptRequest(&server_addr, &client_addr);
//		Receives an incoming message (in theory at least)
		bzero(buff, 256);
		if (server->getNewSocket() >= 0 && recv(server->getNewSocket(), buff, 255, 0) >= 0)
		{
			std::cout << std::endl << "Received Message : " << buff << std::endl;
		}
//		Receives an outgoing message (in theory at least)
		if (server->getNewSocket() >= 0 && send(server->getNewSocket(), "Hello, client!\n", 14, 0) >= 0)
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
