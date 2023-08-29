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
		std::string	name;
		std::string	nick;

	public:
		// Constructors & Destructor
		User();
		User(const std::string _name, const std::string _nick);
		User(const User &other);
		User &operator= (const User &other);
		~User();

		// Getters - Setters
		std::string	getName(void) const;
		std::string	getNick(void) const;
};


#endif // USER_HPP