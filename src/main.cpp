#include "IRC.hpp"

//add them in Server.hpp
static int newSocket = 0;
static int baseSocket = 0;

/**
 * Function to switch our global bool (flag) to stop the infinite loop
 * The infinite loop to keep our IRC server running
 * @param sig
 */
static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << std::endl << std::endl << MAGENTA << " > Closing and cleaning ..." << DEFCOL << std::endl << std::endl;
}

int parseArg(int ac, char **av)
{
    int port;
	if (ac != 3)
		throw std::invalid_argument(" > Error main(): Invalid argument count.");

	std::size_t found = std::string(av[1]).find_first_not_of("1234567890");
	if (found != std::string::npos)
		throw std::invalid_argument(" > Error main(): Not a port");

	port = atoi(av[1]);
	if (port < 6660 || 6669 < port)
		throw std::invalid_argument(" > Error main(): Not a TCP port for IRC");
	if (port < 1025 || 65535 < port)
		throw std::invalid_argument(" > Error main(): Invalid port");
    return (port);
}

int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN); //ignore ctrl-backslash
	signal(SIGINT, stop);

	std::cout << DEFCOL;
	try
	{
        int port = parseArg(ac, av);
        std::string password = av[2];
		Server server(port);

		if (password.compare(server.getPass()) != 0)
			throw std::invalid_argument(" > Error main(): Invalid password");
		//std::cout << std::endl; //  DEBUG
		server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << std::endl << std::endl << RED << e.what() << DEFCOL ;
        //std::cerr << "\n\n"
		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;

		close (baseSocket);
		close (newSocket);

		std::cout << std::endl << std::endl; // DEBUG

		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
