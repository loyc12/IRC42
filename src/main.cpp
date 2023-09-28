#include "IRC.hpp"

#define PASSWORD " > Error main(): Invalid password"
#define PORT " > Error main(): Invalid port"
#define TCP " > Error main(): Not a TCP port for IRC"
#define ARG " > Error main(): Not a port"
#define COUNT " > Error main(): Invalid argument count."

//add them in Server.hpp ??
static int newSocket = 0;
static int baseSocket = 0;

/**
 * Function to switch our global bool (flag) to stop the infinite loop
 * The infinite loop to keep our IRC server running
 * @param signal
 */
static void	stop(int signal)
{
	(void)signal;
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


//	PROGRAM ENTRY POINT
int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, stop);

	int 		 port;
	std::string  password;

	try
	{
		std::cout << DEFCOL;
        port 		= parseArg(ac, av);
		password 	= av[2];

//		Create object server
		Server server(port);
		if (password.compare(server.getPass()) != 0)
			throw std::invalid_argument(PASSWORD);

		server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << "\n\n" << RED << e.what() << "\n" << DEFCOL; //		DEBUG
		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;

		close (baseSocket);
		close (newSocket);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/*

//	<FUNCTION DESCRIPTION> (ALWAYS) & (CAPITALIZED)
	type function (typ arg)
	{
		type	arg;
 														<--- SPACE AFTER INITIAL DECLARATIONS
//		<single line comment> (if needed)
		if (condition) //					WARNING : <description> (if needed)
		{
			multiple;
			lines;
		}
		else if (condition)
			one line;
 														<--- SPACE AFTER ONE LINE CONDITION
		std::cerr << arg << "\n\n"; //		DEBUG
	}
 														<--- SPACE AFTER FUNCTIONS

*/




/*

#ifndef OBJECT_HPP
# define OBJECT_HPP
																	<--- SPACE
# include <stuff.hpp>
																	<--- SPACE
# include "IRC.hpp"
																	<--- SPACE
# define <STUFF>	value
 																	<--- SPACE
class Object
{
	private:
		type	_value;
 																	<--- SPACE
	public:
//		CONSTRUCTORS
		Object();
		~Object();
 																	<--- SPACE
//		SETTERS - GETTERS
		void	setValue(type value) const;
		type	getSender(void) const;
 																	<--- SPACE
		<FONCTION GROUP NAME>
		type	function(type arg); //				DEBUG ???
																	<--- SPACE
		SUBCLASSES
			...
};
 																	<--- SPACE
std::ostream &operator<< (std::ostream &out, const Object &rhs);
 																	<--- SPACE
#endif // OBJECT_HPP

*/
