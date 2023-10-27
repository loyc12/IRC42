#ifndef IRC_HPP
# define IRC_HPP

//Stream
# include <iostream>
# include <sstream>
//Fds
# include <fcntl.h>
# include <unistd.h>
//IP
# include <arpa/inet.h> 
# include <netinet/in.h>
# include <sys/socket.h>
// Tools
# include <string>
# include <map>
# include <list>
# include <vector>
//select()
# include <sys/time.h>
# include <sys/select.h>

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

#endif 
