#ifndef MESSAGE_HPP
# define MESSAGE_HPP

//								OBSOLETE (?)

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
		User		*sender;

	public:
		// Constructors & Destructor
		Message();
		Message(std::string _content, User *sender);
		Message(const Message &other);
		Message &operator= (const Message &other);
		~Message();

		// Getters - Setters
		std::string	getContent(void) const;
		User		*getSender(void) const;
};


#endif // MESSAGE_HPP