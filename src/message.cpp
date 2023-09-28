#include "Message.hpp"

// 0================ BASE FUNCTIONS ================0

Message::Message()
{
	std::cout << YELLOW << ": Called default constructor (MESSAGE) " << DEFCOL;
	this->content = "UNINITIALIZED";
	this->sender = NULL;
}
Message::Message(std::string _content, User *_sender)
{
	std::cout << YELLOW << ": Called parameterized constructor (MESSAGE) " << DEFCOL;

	this->content = _content;
	this->sender = _sender;
}
Message::Message(const Message &other)
{
	std::cout << YELLOW << ": Called copy constructor (MESSAGE) " << DEFCOL;

	this->content = other.getContent();
	this->sender = other.getSender();
}
Message &Message::operator= (const Message &other)
{
	std::cout << YELLOW << ": Called assignment operator (MESSAGE) " << DEFCOL;

	this->content = other.getContent();
	this->sender = other.getSender();

	return *this;
}
Message::~Message()
{
	std::cout << YELLOW << ": Called destructor (MESSAGE) " << DEFCOL;

	// TODO
}
std::ostream &operator<< (std::ostream &out, const Message &rhs)
{
	out << "IRC Message [ content : " << rhs.getContent() << " | sender : " << rhs.getSender() << " ]";

	// TODO

	return (out);
}

// 0================ GETTERS / SETTERS ================0

std::string	Message::getContent(void) const { return (this->content);}
User		*Message::getSender(void) const { return (this->sender);}

// 0================ OTHER FUNCTIONS ================0