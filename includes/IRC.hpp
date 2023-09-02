#ifndef IRC_HPP
# define IRC_HPP

//test.cpp
# include <iostream>
# include <fcntl.h> //		fds
# include <unistd.h> //		close fds
# include <arpa/inet.h> //	inet_ntoa
# include <netinet/in.h> //	IPs
# include <sys/socket.h> //	sockets
# include <string.h> //		string (why not <string> instead ??)

# include "Message.hpp"
# include "Server.hpp"
# include "User.hpp"

# define BUFFSIZE 256

//	put defines here
static bool	stopFlag = false;

//test.cpp
void irc(Server *server);

#endif // IRC_HPP
