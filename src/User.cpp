#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

User::User()
{
	std::cout << ": Called default constructor (USER) ";
	this->name = "UNINITIALIZED";
	this->nick = "UNINITIALIZED";
}
User::User(const std::string _name, const std::string _nick)
{
	std::cout << ": Called parameterized constructor (USER) ";

	this->name = _name;
	this->nick = _nick;
}
User::User(const User &other)
{
	std::cout << ": Called copy constructor (USER) ";

	this->name = other.getName();
	this->nick = other.getNick();
}
User &User::operator= (const User &other)
{
	std::cout << ": Called assignment operator (USER) ";

	this->name = other.getName();
	this->nick = other.getNick();

	return *this;
}
User::~User()
{
	std::cout << ": Called destructor (USER) ";

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