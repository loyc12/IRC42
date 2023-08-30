#ifndef IRC_HPP
# define IRC_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include <stdbool.h>
# include "../includes/Server.hpp"
# include "../includes/User.hpp"
# include "../includes/Message.hpp"

//	put defines here

static bool	stopFlag = false;

void irc(int port, int pass);

#endif // IRC_HPP