#include "IRC.hpp"

//	CTRL_C HANDLING
static void	stop(int signal)
{
	(void)signal;
	shutServ = true;
	debugPrint(MAGENTA, "\n\n > Closing and cleaning ...\n"); //			DEBUG
}

//	COMMAND LINE DATA VERIFICATION
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

//	PROGRAM ENTRY POINT
int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, stop);

	int 		 port;
	std::string  password;

	try
	{
		std::cout << DEFCOL;	//	DEBUG
        port 		= parseArg(ac, av);
		password 	= av[2];
//		Create object server
		Server server(port, password);
		server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << "\n\n" << RED << e.what() << "\n" << DEFCOL;
		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
