#ifndef USER_HPP
# define USER_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include "Message.hpp"

class Message;

class User
{
	private:
		// Data
		std::string	_name;
		std::string	_nick;
		sockaddr_in _client_addr; //when client is connected

	public:
		// Constructors & Destructor
		User();
		//User(client_addr); //constructor that will take the client_addr.sin_port
		User(const std::string _name, const std::string _nick);
		User(const User &other);
		User &operator= (const User &other);
		~User();

		// Getters - Setters
		std::string	getName(void) const;
		std::string	getNick(void) const;
		void		setName(std::string name);
		void		setNick(std::string nick);
};


#endif // USER_HPP