#ifndef USER_HPP
# define USER_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h> // for file descriptors (?)
# include <sys/socket.h>
# include "Message.hpp"

class Message;

class User
{
	private:
		// Data
		std::string	_name;
		std::string _userName;
		std::string	_nick;
		std::string _mode;//maybe char better?
		sockaddr_in _client_addr; //when client is connected
		User();

	public:
		// Constructors & Destructor
		User(sockaddr_in client_addr); //constructor that will take the client_addr.sin_port
		User(const std::string _name, const std::string _nick);
		User(const User &other);
		User &operator= (const User &other);
		~User();

		// Getters - Setters
		std::string	getName(void) const;
		std::string	getNick(void) const;
		sockaddr_in	getClientAddr(void) const;
		std::string getUsername(void) const;
		std::string getMode(void) const;
		void		setName(std::string name);
		void		setNick(std::string nick);
		void		setUsername(std::string userName);
		void		setMode(std::string mode);
		void		parseUserInfo(std::string *args);
};


#endif // USER_HPP