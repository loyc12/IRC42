#include "Message.hpp"

// 0================ BASE FUNCTIONS ================0

Message::Message()
{
	std::cout << ": Called default constructor (MESSAGE) ";
	this->content = "UNINITIALIZED";
	this->sender = NULL;
}
Message::Message(std::string _content, User *_sender)
{
	std::cout << ": Called parameterized constructor (MESSAGE) ";

	this->content = _content;
	this->sender = _sender;
}
Message::Message(const Message &other)
{
	std::cout << ": Called copy constructor (MESSAGE) ";

	this->content = other.getContent();
	this->sender = other.getSender();
}
Message &Message::operator= (const Message &other)
{
	std::cout << ": Called assignment operator (MESSAGE) ";

	this->content = other.getContent();
	this->sender = other.getSender();

	return *this;
}
Message::~Message()
{
	std::cout << ": Called destructor (MESSAGE) ";

	// TODO
}
std::ostream &operator<< (std::ostream &out, const Message &rhs)
{
	out << "IRC Message [ content : " << rhs.getContent() << " | sender : " << rhs.getSender() << " ]";

	// TODO

	return (out);
}

// 0================ OTHER FUNCTIONS ================0

std::string	Message::getContent(void) const
{
	return (this->content);
}
User	*Message::getSender(void) const
{
	return (this->sender);
}

// 0================ OTHER FUNCTIONS ================0