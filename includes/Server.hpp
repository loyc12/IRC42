#ifndef SERVER_HPP
# define SERVER_HPP


# include <iostream>
# include <stdlib.h>		//for which functions ??
# include <fcntl.h>			//for file descriptors (?)
# include <arpa/inet.h>		//inet_ntoa
# include <netinet/in.h>	//socket
# include <sys/socket.h>	//socket (what's the difference between the two?)

Class Server
{
	private:
		// Private Constructor
		Server();

	public:
		// Constructors & Destructor
		Server(const int port, const int password);
		Server(const Server &other);
		Server &operator= (const Server &other);
		~Server();

		// Others
};


#endif // SERVER_HPP