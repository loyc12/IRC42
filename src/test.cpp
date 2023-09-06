#include "IRC.hpp"

static int newSocket = 0;
static int baseSocket = 0;

static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << std::endl << std::endl << " > Closing and cleaning ..." << std::endl << std::endl;
	exit(1); //	here because commands are blocking, preventing flag checks
}


int read_from_client(int fd, std::string *message)
{
	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);
	if (byteReceived < 0 && errno != EAGAIN)
		throw std::invalid_argument(" > Error at recv : ");
	else if (byteReceived == 0)
	{
		std::cout<< std::endl << "CLIENT DISCONNECTED" << std::endl << std::endl;
		return (-1);
	}
	else if (byteReceived)
	{
		message->assign(buff, 0, byteReceived);
		std::cout << *message << std::endl;
	}
	return (0);
}

void irc(Server *server)
{
	fd_set				fdsMaster;
	int 				socketCount;
	std::string 		message;
	struct sockaddr_in	server_addr;
	struct sockaddr_in	client_addr;
	socklen_t 			client_len = sizeof(client_addr);

//	Inits base socket
	baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");
//	bind
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //					bind call
    server_addr.sin_port = htons(server->getPort()); //		conversion to network byte order (Ip adress)
    server_addr.sin_addr.s_addr = INADDR_ANY; //			host ip adress **INADDR_ANY go get localhost
	if (bind(baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");
	listen(baseSocket, SOMAXCONN);
//	prepare fds for select
	FD_ZERO(&fdsMaster);
	FD_SET(baseSocket, &fdsMaster);

//	Client interaction loop
	while (!stopFlag)
	{
		fd_set fdsCopy = fdsMaster;
		socketCount = select(FD_SETSIZE, &fdsCopy, nullptr, nullptr, nullptr);
		if (socketCount == -1)
			throw std::invalid_argument(" > Error at select(): ");
		else if (socketCount) { for (int i = 0; i < FD_SETSIZE; ++i) { if (FD_ISSET(i, &fdsCopy))
		{
			if (i == baseSocket) /*Connection request on original socket*/
			{
				newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len);
				if (newSocket <= 0)
					throw std::invalid_argument(" > Error at accept(): ");
				else
				{
					std::cout << "NEW CLIENT ON SOCKET:" << newSocket
					<< " " << inet_ntoa(client_addr.sin_addr)
					<< ":" << ntohs(client_addr.sin_port) << std::endl << std::endl;
					FD_SET(newSocket, &fdsMaster);
				}
			}
			else /*Read message on the known client*/
			{
				if (read_from_client(i, &message) < 0)
				{
					close(i);
					FD_CLR(i, &fdsMaster);
				}
			}
		}}}
	}
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
        if (port < 6660|| 6669 < port)
            throw std::invalid_argument(" > Error main(): Not a TCP port for IRC");
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
