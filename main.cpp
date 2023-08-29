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
	int socket_fd;
    //structure for sockets
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

    //create a socket : Doc -> man ip (7)
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << " Error at socket();" << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Socket() is OK!" << std::endl;

	//Bind procedure (clear & setup)
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//bind call
    serv_addr.sin_addr.s_addr = INADDR_ANY;//host ip adress **INADDR_ANY go get localhost
    serv_addr.sin_port = htons(port);//conversion to network byte order (Ip adress)
	if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << " Error at bind();" << std::strerror(errno) << std::endl;
		exit(1);
	}
	else
		std::cout << "Bind() is OK!" << std::endl;

	//Listen with a backlog queue of 8.
	listen(socket_fd, 8);


	/*-------trying out to do the loop to connect multiple clients------*/
/*from tuto*/
	// fd_set origin;
	// FD_ZERO(&origin); //to put it as empty
	// FD_SET(socket_fd, &origin);
	// fd_set cpyOri = origin; //copy fd_set struct; select() will destroy it
	// int	socketNb = select(0, &cpyOri, nullptr, nullptr, nullptr); //first param: no of sockets to be checked... really not sure...
	// for (int i = 0; i < socketNb; i++)
	// {
	// 	//first prob as I'm focusing too much on the tuto and I have to adapt it for macOS
	// }
/*from tuto*/
	//might need to add poll() somewhere... before loop??
	while (true)
	{
		//Accept
		socklen_t	cli_len = sizeof(cli_addr);
		new_sock_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &cli_len);
		if (new_sock_fd < 0)
		{
			std::cerr << " Error at accept();" << std::strerror(errno) << std::endl;
			exit(1);
		}
		else
			std::cout << "Accept() is OK!" << std::endl;

		//anwser from server if got here:
		std::cout << "Got a connection by address : " << inet_ntoa(cli_addr.sin_addr) << " ( port " << ntohs(cli_addr.sin_port) << " )" << std::endl;

		// This send() function sends the 14 bytes of the string to the new socket
		send(new_sock_fd, "Hello, client!\n", 14, 0); //14 can be replace by strlen

		//setting up message buffer
		char buffer[256];
		bzero(buffer, 256);

		int n = read(new_sock_fd, buffer, 255);//why 255? not 256? should we use strlen of buffer?
		if (n < 0)
			throw "Socket reading failure";

		std::cout << "here is the message: " << buffer << std::endl;

		n = fcntl(new_sock_fd, F_SETFL, "I got your message\n", O_NONBLOCK); //?
		// n = write(new_sock_fd,"I got your message", 18); // use fcntl() instead of write()
		if (n < 0)
			throw "Socket writing failure";
	}


	close (socket_fd);
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
