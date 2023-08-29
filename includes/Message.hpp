#ifndef MESSAGE_HPP
# define MESSAGE_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h>			// for file descriptors (?)
# include "User.hpp"

class User;

class Message
{
	private:
		// Data
		std::string	content;
		User		sender;

		// Private Constructor
		Message();

	public:
		// Constructors & Destructor
		Message(const int port, const int password);
		Message(const Message &other);
		Message &operator= (const Message &other);
		~Message();

		// Others
};


#endif // MESSAGE_HPP