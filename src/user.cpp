#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

User::User()
{
	std::cout << YELLOW << ": Called default constructor (USER) " << DEFCOL;
	this->name = "UNINITIALIZED";
	this->nick = "UNINITIALIZED";
}
User::User(const std::string _name, const std::string _nick)
{
	std::cout << YELLOW << ": Called parameterized constructor (USER) " << DEFCOL;

	this->name = _name;
	this->nick = _nick;
}
User::User(const User &other)
{
	std::cout << YELLOW << ": Called copy constructor (USER) " << DEFCOL;

	this->name = other.getName();
	this->nick = other.getNick();
}
User &User::operator= (const User &other)
{
	std::cout << YELLOW << ": Called assignment operator (USER) " << DEFCOL;

	this->name = other.getName();
	this->nick = other.getNick();

	return *this;
}
User::~User()
{
	std::cout << YELLOW << ": Called destructor (USER) " << DEFCOL;

	// TODO
}
std::ostream &operator<< (std::ostream &out, const User &rhs)
{
	out << "IRC User [ name : " << rhs.getName() << " | nick : " << rhs.getNick() << " ]";

	// TODO

	return (out);
}

// 0================ GETTERS / SETTERS ================0

std::string	User::getName(void) const { return (this->name);}
std::string	User::getNick(void) const { return (this->nick);}

// 0================ OTHER FUNCTIONS ================0
