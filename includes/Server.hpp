#ifndef SERVER_HPP
# define SERVER_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include <list>
# include "User.hpp"
# include "Message.hpp"

/*
# include <arpa/inet.h>		// inet_ntoa
# include <netinet/in.h>	// socket
# include <sys/socket.h>	// socket (what's the difference between the two?)
*/


class User;
class Message;

class Server
{
	private:
		// Data
		int	port;
		int	pass;
		std::list<int>				userLst; //will need to change it to a list of pointers to instance of class User
		std::list<int>::iterator	it; //iterator so we can go through

		// Private Constructor

	public:
		// Constructors & Destructor
		Server();
		Server(int _port, int _pass);
		Server(const Server &other);
		Server &operator= (const Server &other);
		~Server();

		// Getters - Setters
		int	getPort(void) const ;
		int	getPass(void) const;
		void	irc(int port, int pass);
};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP