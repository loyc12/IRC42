#ifndef SERVER_HPP
# define SERVER_HPP

<<<<<<< HEAD

# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include <list>
=======
>>>>>>> master
# include "User.hpp"
# include "Message.hpp"

class User;
class Message;

class Server
{
	private:
		// Data
<<<<<<< HEAD
		int	port;
		int	pass;
		std::list<int>				userLst; //will need to change it to a list of pointers to instance of class User
		std::list<int>				clientSockets; //tmp to tryout something
		std::list<int>::iterator	it; //iterator so we can go through

=======
		int	_port;
		int _newSocketFd;
		int _baseSocketFd;
>>>>>>> master
		// Private Constructor

	public:
		// Constructors & Destructor
<<<<<<< HEAD
		Server();
		Server(int _port, int _pass);
=======
		Server(int port);
>>>>>>> master
		Server(const Server &other);
		Server &operator= (const Server &other);
		~Server();

		// Getters - Setters
<<<<<<< HEAD
		int	getPort(void) const ;
		int	getPass(void) const;
		void	irc(int port, int pass);
=======
		const int &	getPort(void) const ;
		const int & getBaseSocket(void) const;
		const int & getNewSocket(void) const;
		void		setBaseSocket(int baseSocket);
		void		setNewSocket(int newSocket);

		void initSocket(void);
		void initBind(struct sockaddr_in *server_addr);
		void acceptRequest(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr);
		// receiveMessage(void);
		// sendMessage(void);
>>>>>>> master
};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
