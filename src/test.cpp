#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h> //		file descriptor

#include <arpa/inet.h> //	inet_ntoa
#include <netinet/in.h> //	socket
#include <sys/socket.h> //	socket
#include "IRC.hpp"

int new_socket_fd = 0; //	placer ceci dans serv
int base_socket_fd = 0; //	placer ceci dans serv

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

//	Data for sockets and shit
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	char 				buff[256]; //	FOR MESSAGE RECEIVING/SENDING

//	Create a socket : Doc -> man ip (7)
	base_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (base_socket_fd < 0)
		throw std::invalid_argument(" > Error at socket(): ");
	else
		std::cout << "Socket() is OK!" << std::endl;

//	Makes it so that read/write call to sockets return -1 if blocking
	if (fcntl(base_socket_fd, F_SETFL, O_NONBLOCK))
		throw std::invalid_argument(" > Error at fcntl() : ");
	else
		std::cout << "fcntl() is OK!" << std::endl;

//	Bind procedure (clear & setup)
	bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; //			bind call
    server_addr.sin_addr.s_addr = INADDR_ANY; //	host ip adress **INADDR_ANY go get localhost
    server_addr.sin_port = htons(port); //			conversion to network byte order (Ip adress)

	if (bind(base_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");
	else
		std::cout << "Bind() is OK!" << std::endl;

//	Listen with a backlog queue of N.
	listen(base_socket_fd, 16);

/*
//	EXAMPLE CODE FOR MAIN WHILE LOOP GOAL

	while (!stopFlag)
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

//		bzero(buff, 256);
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
		close (base_socket_fd);
		close (new_socket_fd);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
