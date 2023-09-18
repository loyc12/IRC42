#ifndef IRC_HPP
# define IRC_HPP

//test.cpp
# include <iostream>
# include <sstream>
# include <fcntl.h> //		fds
# include <unistd.h> //		close fds
# include <arpa/inet.h> //	IP stuff
# include <netinet/in.h> //	IP stuff
# include <sys/socket.h> //	socket stuff
# include <string.h> //		string (why not <string> instead ??)
# include <string> //		string (why not <string.h> instead ??)
# include <map>

# include <sys/time.h> //	select
# include <sys/select.h> //	select

# include "Colours.hpp"
# include "Message.hpp"
# include "Server.hpp"
# include "User.hpp"

# define BUFFSIZE 256

//	put defines here
static bool	stopFlag = false;

//test.cpp
void irc(Server *server);



/*
class IRC
Just to have an instance in the main and in private, it has an instance of server
*/

#endif // IRC_HPP
