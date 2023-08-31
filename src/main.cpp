#include "IRC.hpp"
#include <unistd.h>

// int new_sock_fd;//deplacer à l'exterieur juste pour voir si stop fonctionne
// int socket_fd;

// static void	stop(int sig)
// {
// 	//switch our global to stop the infinite loop
// 	(void)sig;
// 	stopFlag = true;
// 	close(socket_fd);
// 	close(new_sock_fd);
// 	write(2, "a", 1);
// }

// int	main(int ac, char **av)
// {
// 	// signal(SIGQUIT, SIG_IGN);//reset signal
// 	// signal(SIGINT, stop);
// 	try
// 	{
// 		if (ac != 3)
// 			throw "Invalid argument count";

// 		//arg parsing
// 		irc(atoi(av[1]), atoi(av[2]));
// 	}
// 	/*catch (std::string &msg)	//exceptions are char const * for now
// 	{
// 		std::cerr << "ERROR : " << msg << std::endl;
// 		exit(EXIT_FAILURE);
// 	}*/
// 	catch (char const *msg)
// 	{
// 		std::cerr << "ERROR : " << msg << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// 	exit(EXIT_SUCCESS);
// }
