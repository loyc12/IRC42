
#include "IRC.hpp"

int new_socket_fd = 0; //copie dans serv
int base_socket_fd = 0; //copie dans serv

static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << "\n > Closing and cleaning ..." << std::endl;
}

void irc(int port, int pass)
{
	(void) pass;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	char 				buff[256]; //	FOR MESSAGE RECEIVING/SENDING

	Server server(port);
	server.initSocket();
	server.initBind(&server_addr);
	listen(server.getBaseSocket(), 16);
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
	close (base_socket_fd);
	close (new_socket_fd);
*/

	// Client interaction loop
	while (!stopFlag)
	{
		server.acceptRequest(&server_addr, &client_addr);
//		Receives an incoming message (in theory at least)
		bzero(buff, 256);
		if (new_socket_fd >= 0 && recv(new_socket_fd, buff, 255, 0) >= 0)
		{
			std::cout << std::endl << "Received Message : " << buff << std::endl;
		}
//		Receives an outgoing message (in theory at least)
		if (new_socket_fd >= 0 && send(new_socket_fd, "Hello, client!\n", 14, 0) >= 0)
		{
			std::cout << std::endl << "Sent Message : " << "Hello, client!\n" << std::endl;
		}
	}
	close (base_socket_fd);
	close (new_socket_fd);
}

int	main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN); //	resets signal
	signal(SIGINT, stop);
	try
	{
		if (ac != 3)
			throw std::invalid_argument(" > Error main(): Invalid argument count.");
//		Arg parsing
		irc(atoi(av[1]), atoi(av[2]));
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		if (errno)
			std::cout << std::strerror(errno) << std::endl;
		/*
		close(getBaseSocket());
		close(getNewSocket());
		*/
		close (base_socket_fd);
		close (new_socket_fd);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
