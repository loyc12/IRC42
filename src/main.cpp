#include "IRC.hpp"

Server	*getServer(void)
{
	static Server	*server = NULL;

	if (server == NULL)
		server = new Server();
	return (server);
}

//	CTRL_C HANDLING
static void	stop(int signal)
{
	(void)signal;
	getServer()->shutoff = true;
	debugPrint(MAGENTA, "\n\n > Closing...\n"); //					DEBUG
}

//	PROGRAM ENTRY POINT
int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, stop);

	std::cout << DEFCOL;	//										DEBUG

	int  port;
	int  pass;

	try
	{
		if (ac != 3)
			throw std::invalid_argument(COUNT);
		else if (std::string(av[1]).find_first_not_of("1234567890") != std::string::npos)
			throw std::invalid_argument(PORT);
		else if (std::string(av[2]).find_first_not_of("1234567890") != std::string::npos)
			throw std::invalid_argument(PASS);

		pass = atoi(av[2]);
		port = atoi(av[1]);
		if (port < 6660 || 6669 < port) // (port < 1025 || 65535 < port) Used those for IRC
			throw std::invalid_argument(TCP);

//		Create object server
		Server *server = getServer();
		server->setPort(port);
		server->setPass(pass);
		server->run();
		server->clear(); //need to comment out when check for leaks
	}
	catch (std::exception &e)
	{
		std::cerr << "\n\n" << RED << e.what() << "\n" << DEFCOL;
		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;
		getServer()->clear();
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
