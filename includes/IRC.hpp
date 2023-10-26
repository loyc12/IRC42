#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <sstream>
# include <fcntl.h> //		fds
# include <unistd.h> //		close fds
# include <arpa/inet.h> //	IP stuff
# include <netinet/in.h> //	IP stuff
# include <sys/socket.h> //	socket stuff
# include <string.h> //		string (why not <string> instead ??)// REVIEW
# include <string> //		string (why not <string.h> instead ??)// REVIEW
# include <map>
# include <list>
# include <vector> //		container of users found in a channel

# include <sys/time.h> //	select
# include <sys/select.h> //	select

class Server;
class Channel;
class User;

# include "Colours.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "User.hpp"
# include "Utils.hpp"
# include "Codes.hpp"

// PUT NON_CODE DEFINES HERE
# define BUFFSIZE 256

//  ERROR
# define PASS		" > Error main(): Invalid password"
# define PORT		" > Error main(): Invalid port"
# define TCP		" > Error main(): Not a TCP port for IRC"
# define COUNT		" > Error main(): Invalid argument count."

Server	*getServer(void); //	singleton used to acces the server instance from anywhere

#endif // IRC_HPP
