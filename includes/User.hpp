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
		// Private Constructor
		User();

	public:
		// Constructors & Destructor
		User(const int port, const int password);
		User(const User &other);
		User &operator= (const User &other);
		~User();

		// Others
};


#endif // USER_HPP