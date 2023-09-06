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
	struct sockaddr_in	server_addr, client_addr;
	socklen_t 			client_len = sizeof(client_addr);

//	Inits base socket
	baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");
	else
		std::cout << "socket() :   OK!" << std::endl;

//	bind
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //					bind call
    server_addr.sin_port = htons(server->getPort()); //		conversion to network byte order (Ip adress)
    server_addr.sin_addr.s_addr = INADDR_ANY; //			host ip adress **INADDR_ANY go get localhost
	if (bind(baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");
	else
		std::cout << "bind() :     OK!" << std::endl;

//	listen
	listen(baseSocket, SOMAXCONN);

	//select
	char 	buff[BUFFSIZE];
	fd_set	fdsMaster, fdsRead; //, fdWrite;
	int 	socketCount;
	FD_ZERO(&fdsMaster);
	FD_SET(baseSocket, &fdsMaster);

//	Client interaction loop
	while (!stopFlag)
	{
		fdsRead = fdsMaster;
//		fd_set fdsWrite = fdsMaster; // for eventual reading; third argument of select()

//		still blocking ...
		socketCount = select(baseSocket + 1, &fdsRead, nullptr, nullptr, nullptr);

		if (socketCount == -1)
			throw std::invalid_argument(" > Error at select(): ");
		else if (socketCount)
		{
			std::cout << "select() :   OK!" << std::endl; //	Listening socket is ready ...

            if (FD_ISSET(baseSocket, &fdsRead)) //  judges if a fd is ready for interactions (aka; won't block)
			{
				std::cout << "FD_ISSET() : OK!" << std::endl;

				newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len);

                if (newSocket < 0)
					throw std::invalid_argument(" > Error at accept(): ");
				else if (newSocket)
				{
					std::cout << "accept() :   OK!" << std::endl;
					std::cout << "    new socket fd : " << newSocket << std::endl; // gets a new socket fd to represent the every new (re)connections

					std::cout << std::endl << "0========== CLIENT CONNECTED =========0" << std::endl << std::endl;

					//			Receives any potential message from client
					while (!stopFlag)
					{
						bzero(buff, BUFFSIZE);
						int byteCount = recv(newSocket, buff, BUFFSIZE - 1, MSG_DONTWAIT);
						if (byteCount < 0 && errno != EAGAIN)
							throw std::invalid_argument(" > Error at recv : ");
						else if (byteCount == 0)
						{
							std::cout<< std::endl << "0======== CLIENT DISCONNECTED ========0" << std::endl << std::endl;
							close(newSocket); //  buggy ?
							newSocket = 0;
                            break;
                        }
						else if (byteCount)
						{
							std::cout << std::string(buff, 0, byteCount);
						}
					}
				}
			}
			else
				throw std::invalid_argument(" > Error at accept(): ");
		}
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
        std::cerr << std::endl << std::endl << e.what();
        if (errno)
            std::cout << std::strerror(errno) << std::endl;
        close (baseSocket);
        close (newSocket);
        std::cout << std::endl << std::endl; // DEBUG
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}