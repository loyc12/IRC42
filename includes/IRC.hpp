#ifndef IRC_HPP
# define IRC_HPP

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
# include <list>
# include <vector> //		container of users found in a channel

# include <sys/time.h> //	select
# include <sys/select.h> //	select

class Server;
class Channel;
class User;
//class Message;

# include "Colours.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "User.hpp"
# include "Utils.hpp"
//# include "Message.hpp"

// PUT NON_CODE DEFINES HERE
# define BUFFSIZE 256

//  ERROR
# define PASSWORD	" > Error main(): Invalid password"
# define PORT		" > Error main(): Invalid port"
# define TCP		" > Error main(): Not a TCP port for IRC"
# define ARG		" > Error main(): Not a port"
# define COUNT		" > Error main(): Invalid argument count."

//	put defines here
static bool	shutServ = false;

//void irc(Server *server);
int parseArg(int ac, char **av);


/*
class IRC
Just to have an instance in the main and in private, it has an instance of server
*/

#endif // IRC_HPP
