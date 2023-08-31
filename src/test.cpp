#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>//file descriptor

#include <arpa/inet.h>//inet_ntoa
#include <netinet/in.h>//socket
#include <sys/socket.h>//socket
#include "IRC.hpp"

	int new_socket_fd;//deplacer à l'exterieur juste pour voir si stop fonctionne
	int base_socket_fd;


static void	stop(int sig)
{
	//switch our global to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << "\nCLosing and cleaning..." << std::endl;
}

void irc(int port, int pass)
{
	(void) pass;

    //data for sockets and shit
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	char 				buff[256]; //	FOR MESSAGE RECEIVING/SENDING

    //create a socket : Doc -> man ip (7)
	base_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (base_socket_fd < 0)
	{
		std::cerr << " > Error at socket(): " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Socket() is OK!" << std::endl;

	//makes it so that read/write call to sockets return -1 if blocking
	if (fcntl(base_socket_fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << " > Error at fcntl() : " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "fcntl() is OK!" << std::endl;

	// Bind procedure (clear & setup)
	bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;//bind call
    server_addr.sin_addr.s_addr = INADDR_ANY;//host ip adress **INADDR_ANY go get localhost
    server_addr.sin_port = htons(port);//conversion to network byte order (Ip adress)

	if (bind(base_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << " > Error at bind(): " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Bind() is OK!" << std::endl;

	// Listen with a backlog queue of N.

	listen(base_socket_fd, 16);

	// Client interaction loop (???)
	while (!stopFlag)
	{

//		Accepts an incoming connection request
		new_socket_fd = accept(base_socket_fd, (struct sockaddr *) &client_addr, &client_len);
		if (new_socket_fd < -1)
		{
			std::cerr << " > Error at accept(): " << std::strerror(errno) << std::endl;
			break;
		}
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
	signal(SIGQUIT, SIG_IGN);//reset signal
	signal(SIGINT, stop);
	try
	{
		if (ac != 3)
			throw "Invalid argument count";

		//arg parsing
		irc(atoi(av[1]), atoi(av[2]));
	}
	/*catch (std::string &msg)	//exceptions are char const * for now
	{
		std::cerr << "ERROR : " << msg << std::endl;
		exit(EXIT_FAILURE);
	}*/
	catch (char const *msg)
	{
		std::cerr << "ERROR : " << msg << std::endl;
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
