#ifndef IRC_HPP
# define IRC_HPP

//test.cpp
#include <iostream>
#include <fcntl.h>//fd
#include <unistd.h>//close
#include <arpa/inet.h> //	inet_ntoa
#include <netinet/in.h> //	socket
#include <sys/socket.h> //	socket

# include "Server.hpp"
# include "User.hpp"
# include "Message.hpp"

//	put defines here

static bool	stopFlag = false;

//test.cpp
void irc(int port, int pass);

#endif // IRC_HPP
