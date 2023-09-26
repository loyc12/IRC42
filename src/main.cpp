#include "IRC.hpp"

#define PASSWORD " > Error main(): Invalid password"
#define PORT " > Error main(): Invalid port"
#define TCP " > Error main(): Not a TCP port for IRC"
#define ARG " > Error main(): Not a port"
#define COUNT " > Error main(): Invalid argument count."

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
	(void)sig;
	shutServ = true;
	std::cout << "\n\n" << MAGENTA << " > Closing and cleaning ..." << DEFCOL << "\n" << std::endl;
}

int parseArg(int ac, char **av)
{
    int 		port;
	std::size_t found;

	if (ac != 3)
		throw std::invalid_argument(COUNT);

	found = std::string(av[1]).find_first_not_of("1234567890");
	if (found != std::string::npos)
		throw std::invalid_argument(ARG);

	port = atoi(av[1]);
	if (port < 6660 || 6669 < port)
		throw std::invalid_argument(TCP);
	if (port < 1025 || 65535 < port)
		throw std::invalid_argument(PORT);

    return (port);
}

int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, stop);

	int 		 port;
	std::string  password;
	std::cout << DEFCOL;

	try {
        port = parseArg(ac, av);
        password = av[2];

//		Create object server
		Server server(port);
		if (password.compare(server.getPass()) != 0)
			throw std::invalid_argument(PASSWORD);
		server.start();
	}
	catch (std::exception &e) {

		std::cerr << std::endl << std::endl << RED << e.what() << "\n" << DEFCOL;
		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;
		close (baseSocket);
		close (newSocket);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
