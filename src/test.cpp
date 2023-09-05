#include "IRC.hpp"

static int newSocket = 0;
static int baseSocket = 0;

static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << std::endl << std::endl << " > Closing and cleaning ..." << std::endl << std::endl;
//	exit(1); //	here because commands are blocking, preventing flag checks
}

void irc(Server *server)
{
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	socklen_t 			client_len = sizeof(client_addr);

//	Inits base socket
	baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");
	else
		std::cout << "socket() is OK!" << std::endl;

//	bind
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //					bind call
    server_addr.sin_port = htons(server->getPort()); //		conversion to network byte order (Ip adress)
    server_addr.sin_addr.s_addr = INADDR_ANY; //			host ip adress **INADDR_ANY go get localhost
	if (bind(baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");
	else
		std::cout << "bind() is OK!" << std::endl;

//	listen
	listen(baseSocket, SOMAXCONN);

	//select
	char 	buff[BUFFSIZE];
	fd_set	fdsMaster;
	int 	socketCount;
	FD_ZERO(&fdsMaster);
	FD_SET(baseSocket, &fdsMaster);

//	Client interaction loop
	while (!stopFlag)
	{
		fd_set fdsRead = fdsMaster;
//		fd_set fdsWrite = fdsMaster;

		socketCount = select(baseSocket + 1, &fdsRead, nullptr, nullptr, nullptr);

		if (socketCount < 0)
			throw std::invalid_argument(" > Error at select(): ");
		else if (socketCount > 0)
		{
			std::cout << "select() is OK and > 0 !" << std::endl; //									DEBUG
			std::cout << "\tListening socket is ready." << std::endl; //								DEBUG

			if (FD_ISSET(baseSocket, &fdsRead))
			{
				std::cout << "FD_ISSET() is OK!" << std::endl; //										DEBUG
				std::cout << "\tFd " << newSocket << " can be read without blocking" << std::endl; //	DEBUG

				newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len);

				if (newSocket < 0)
					throw std::invalid_argument(" > Error at accept(): ");
				else if (newSocket > 0)
				{
					std::cout << std::endl << "Accept is okay !" << std::endl; //						DEBUG
					std::cout << std::endl << "newSocket : " << newSocket << std::endl; //				DEBUG
					std::cout << std::endl << "Client connected !" << std::endl << std::endl; //		DEBUG

	//				Receives any potential message from client
					bzero(buff, BUFFSIZE);
					int byteReceived = recv(newSocket, buff, BUFFSIZE - 1, 0);

					std::cout << byteReceived << " ( " << errno << " )" << std::endl << std::endl; //	DEBUG

					if (byteReceived == -1 && errno != EAGAIN) //			is EAGAIN check allowed ??? else must use select()
						throw std::invalid_argument(" > Error at recv : ");
					else if (byteReceived == 0)
					{
						std::cout<< std::endl << "Client disconnected ..." << std::endl << std::endl;
						close(newSocket);
						newSocket = 0;
						std::cout << std::endl << "Awaiting new request from client at : " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
					}
					else if (byteReceived > 0)
						std::cout << std::string(buff, 0, byteReceived) << std::endl;

				}
			}
//			else
//				usleep(10000);
		}
	}



//	( )================ OLD VERSION ================( )

/*
//	Waits for a client connection
	std::cout << std::endl << "Awaiting request from client at : " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
	newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len); //	is blocking
	std::cout << "Client connected !" << std::endl << std::endl;
	close(baseSocket);

	char 	buff[BUFFSIZE];

	// Client interaction loop
	while (stopFlag == false)
	{
		bzero(buff, BUFFSIZE);
		int byteReceived = recv(newSocket, buff, BUFFSIZE - 1, 0); //	is blocking
		if (byteReceived == -1)
			throw std::invalid_argument(" > Error at recv :");
		if (byteReceived == 0)
		{
			std::cout << std::endl << "Client disconnected ..." << std::endl << std::endl;
			break ;
		}
        std::cout <<  std::string(buff, 0, byteReceived);
	}
*/

	close(baseSocket);
	close(newSocket);
}

int main(int ac, char **av)
{
    signal(SIGQUIT, SIG_IGN); //ignore ctrl-backslash
    signal(SIGINT, stop);
    try
    {
        if (ac != 3)
            throw std::invalid_argument(" > Error main(): Invalid argument count.");
        std::string firstArg = av[1];
        std::size_t found = firstArg.find_first_not_of("1234567890");
        if (found != std::string::npos)
            throw std::invalid_argument(" > Error main(): Not a port");
        int port = atoi(av[1]);
        if (port < 1025 || 65535 < port)
            throw std::invalid_argument(" > Error main(): Invalid port");
        int pass = atoi(av[2]);
        (void) pass;
        Server server(port);
        std::cout << std::endl; //  DEBUG
        irc(&server);
    }
    catch (std::exception &e)
    {
        std::cerr << std::endl << e.what();
        if (errno)
            std::cout << std::strerror(errno) << std::endl;
        close (baseSocket);
        close (newSocket);
        std::cout << std::endl << std::endl; // DEBUG
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}