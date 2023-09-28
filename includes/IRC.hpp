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
# include "Utils.hpp"

# define BUFFSIZE 256

//  ERROR
        #define PASSWORD " > Error main(): Invalid password"
        #define PORT     " > Error main(): Invalid port"
        #define TCP      " > Error main(): Not a TCP port for IRC"
        #define ARG      " > Error main(): Not a port"
        #define COUNT    " > Error main(): Invalid argument count."

//	put defines here
static bool	shutServ = false;

//test.cpp
void irc(Server *server);
int parseArg(int ac, char **av);


/*
class IRC
Just to have an instance in the main and in private, it has an instance of server
*/

#endif // IRC_HPP
