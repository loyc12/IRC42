#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>//file descriptor

#include <arpa/inet.h>//inet_ntoa
#include <netinet/in.h>//socket
#include <sys/socket.h>//socket

void irc(int port, int pass)
{
	(void) pass;
	int new_sock_fd;
	SOCKET serverSocket = INVALID_SOCKET;

    //create a socket : Documentation for socket() : man ip (7)
	// int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	//! AV : Bound the result in the class SOCKET instead of a int
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cerr << " Error at socket();" << std::strerror(errno) << std::endl;
		exit(1);
		//^ Need to talk to setting up error management.
	}
	else
		std::cout << "Socket() is OK!" << std::endl;

    //structure for sockets
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	bzero((char *) &serv_addr, sizeof(serv_addr));

    //setup structure
    serv_addr.sin_family = AF_INET;//bind call
    serv_addr.sin_addr.s_addr = INADDR_ANY;//host ip adress
    serv_addr.sin_port = htons(port);//conversion to network byte order (Ip adress)

    //bind the socket to the current IP address on port
	if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)//recast //PROTECT
    	throw "Binding failure";

	//waiting for request
	listen(sock_fd, 8); // max 8 pending conections at a time

	//accepting request
	socklen_t	cli_len = sizeof(cli_addr);
	new_sock_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &cli_len);
    if (new_sock_fd < 0)
        throw "Accept failure";

    //anwser from server if got here:
    std::cout << "Got a connection by address : " << inet_ntoa(cli_addr.sin_addr) << " ( port " << ntohs(cli_addr.sin_port) << " )" << std::endl;

    // This send() function sends the 14 bytes of the string to the new socket
    send(new_sock_fd, "Hello, client!\n", 14, 0);

	//setting up message buffer
	char buffer[256];
	bzero(buffer, 256);

    int n = read(new_sock_fd, buffer, 255);
    if (n < 0)
		throw "Socket reading failure";

    std::cout << "here is the message: " << buffer << std::endl;

	n = fcntl(new_sock_fd, F_SETFL, "I got your message\n", O_NONBLOCK); //?
    // n = write(new_sock_fd,"I got your message", 18); // use fcntl() instead of write()
    if (n < 0)
		throw "Socket writing failure";

	close (sock_fd);
	close (new_sock_fd);
}


int	main(int ac, char **av)
{
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
