#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>//file descriptor

#include <arpa/inet.h>//inet_ntoa
#include <netinet/in.h>//socket
#include <sys/socket.h>//socket
#include "IRC.hpp"

	int new_sock_fd;//deplacer à l'exterieur juste pour voir si stop fonctionne
	int socket_fd;


static void	stop(int sig)
{
	//switch our global to stop the infinite loop
	(void)sig;
	stopFlag = true;
//	close(socket_fd);
//	close(new_sock_fd);
	std::cout << std::endl << "and clean..." << std::endl;
}

void irc(int port, int pass)
{
//	signal(SIGQUIT, SIG_IGN);//reset signal
//	signal(SIGINT, &handler_init_sig);
	(void) pass;
    //structure for sockets
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

    //create a socket : Doc -> man ip (7)
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << " > Error at socket(): " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Socket() is OK!" << std::endl;

	//makes it so that read/write call to sockets return -1 if blocking
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << " > Error at fcntl() (socket_fd): " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "fcntl() (socket_fd) is OK!" << std::endl;

	//Bind procedure (clear & setup)
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//bind call
    serv_addr.sin_addr.s_addr = INADDR_ANY;//host ip adress **INADDR_ANY go get localhost
    serv_addr.sin_port = htons(port);//conversion to network byte order (Ip adress)
	if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << " > Error at bind(): " << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Bind() is OK!" << std::endl;

	//Listen with a backlog queue of 8.

	listen(socket_fd, 8);

	//Accept
	while (!stopFlag) //change to global
	{
//		signal(SIGINT, handler_in_loop);
//		signal(SIGQUIT, handler_in_loop); //good place to put or not?
		socklen_t	cli_len = sizeof(cli_addr);
		new_sock_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &cli_len);
		if (new_sock_fd < -1)
		{
			std::cerr << " > Error at accept(): " << std::strerror(errno) << std::endl;
			break;
		}
		else if (new_sock_fd != -1)
		{
			//Print Connection
			std::cout << "\nCONNECTED\nCLIENT FROM NETWORK :\t" << inet_ntoa(cli_addr.sin_addr) << "\nNET-TO-HOST PORT :\t" << ntohs(cli_addr.sin_port) << std::endl;
			std::cout << "\nSERVER IP (local): \t" << inet_ntoa(serv_addr.sin_addr) << "\nHOST-TO-NET PORT :\t" << ntohs(serv_addr.sin_port) << std::endl;

//			TODO : RECEIVE THE MESSAGE (read ?)
			char buff[256];
			bzero(buff, 256);
			read(new_sock_fd, buff, 255);
			std::cout << "Message : " << buff << std::endl;
		}

		// This send() function sends the 14 bytes of the string to the new socket
		if ((send(new_sock_fd, "Hello, client!\n", 14, 0)) < -1)
		{
			std::cerr << " > Error at send(): " << std::strerror(errno) << std::endl;
			break;
		}
		else if (new_sock_fd != -1)
		{
			std::cout << "\nSENDING MESSAGE ..." << std::endl;

//			TODO : SEND THE MESSAGE (write ?)
			write(new_sock_fd, "pong\n", 5);
		}
	}
//	std::cout << "and clean..." << std::endl;
	close (socket_fd);
	close (new_sock_fd);
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
