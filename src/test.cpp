
#include "IRC.hpp"
#include <string>
#include <string.h>

static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	//std::cout << stopFlag << std::endl;
	std::cout << "\n > Closing and cleaning ..." << std::endl;
	exit(1);//flag change pas , ducktape solution
}

void irc(Server *server)
{
	int baseSocket;
	int newSocket;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;

	baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");
	else
		std::cout << "Socket() is OK!" << std::endl;


	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //			bind call
    server_addr.sin_addr.s_addr = INADDR_ANY; //	host ip adress **INADDR_ANY go get localhost
    server_addr.sin_port = htons(server->getPort()); //			conversion to network byte order (Ip adress)
	if (bind(baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");
	else
		std::cout << "Bind() is OK!" << std::endl;

	listen(baseSocket, 16);
	// Client interaction loop
	std::cout << "Request Waiting from client " << inet_ntoa(client_addr.sin_addr) << ntohs(client_addr.sin_port) << std::endl;
	socklen_t client_len = sizeof(client_addr);
	newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len);
	close(baseSocket);
	char 				buff[256]; //	FOR MESSAGE RECEIVING/SENDING

	while (stopFlag == false)//flag does not change
	{
		bzero(buff, 256);
		int byteReceived = recv(newSocket, buff, 255, 0);
		if (byteReceived == -1)
			throw std::invalid_argument(" > Error at recv :");
		if (byteReceived == 0)
		{
			std::cout << "client disconnected" << std::endl;
			break ;
		}
        std::cout << std::string(buff, 0, byteReceived) << std::endl;
	}
	close (baseSocket);
	close (newSocket);

}
int	main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN); //ignore ctrl-backslash
	signal(SIGINT, stop);
	try
	{
		std::string firstArg = av[1];
		std::size_t found = firstArg.find_first_not_of("1234567890");
		if ( found != std::string::npos)
			throw std::invalid_argument(" > Error main(): Not a port");
		int port = atoi(av[1]);
		if ((port >= 0 && port <= 1024) || (port >= 65536))
			throw std::invalid_argument(" > Error main(): Not available port");

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
