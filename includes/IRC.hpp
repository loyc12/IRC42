#ifndef IRC_HPP
# define IRC_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include "Server.hpp"
# include "User.hpp"
# include "Message.hpp"

//	put defines here

static bool	stopFlag = false;

void irc(int port, int pass);

#endif // IRC_HPP