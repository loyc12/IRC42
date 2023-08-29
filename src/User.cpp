#include "User.hpp"

// 0================ BASE FUNCTIONS ================0

User::User()
{
	std::cout << ": Called default constructor (SERVER) ";
	this->name = "UNINITIALIZED";
	this->name = "UNINITIALIZED";
}
User::User(const std::string _name, const std::string _nick)
{
	std::cout << ": Called parameterized constructor (SERVER) ";

	this->name = _name;
	this->nick = _nick;
}
User::User(const User &other)
{
	std::cout << ": Called copy constructor (SERVER) ";

	this->name = other.getName();
	this->nick = other.getNick();
}
User &User::operator= (const User &other)
{
	std::cout << ": Called assignment operator (SERVER) ";

	this->name = other.getName();
	this->nick = other.getNick();

	return *this;
}
User::~User()
{
	std::cout << ": Called destructor (SERVER) ";

	// TODO
}
std::ostream &operator<< (std::ostream &out, const User &rhs)
{
	out << "IRC User [ name : " << rhs.getName() << " | nick : " << rhs.getNick() << " ]";

	// TODO

	return (out);
}

// 0================ OTHER FUNCTIONS ================0

std::string	User::getName(void) const
{
	return (this->name);
}
std::string	User::getNick(void) const
{
	return (this->nick);
}

// 0================ OTHER FUNCTIONS ================0