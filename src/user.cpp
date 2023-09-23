#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

User::User()
{
	std::cout << YELLOW << ": Called default constructor (USER) " << DEFCOL;
	this->_name = "UNINITIALIZED";
	this->_nick = "UNINITIALIZED";
}

User::User(sockaddr_in client_addr) : _client_addr(client_addr) {
	std::cout << YELLOW << ": Called constructor (USER) " << DEFCOL;
	this->_name = "UNINITIALIZED";
	this->_nick = "UNINITIALIZED";//will be changed later on

}


User::User(const std::string _name, const std::string _nick)
{
	std::cout << YELLOW << ": Called parameterized constructor (USER) " << DEFCOL;

	this->_name = _name;
	this->_nick = _nick;
}
User::User(const User &other)
{
	std::cout << YELLOW << ": Called copy constructor (USER) " << DEFCOL;

	this->_name = other.getName();
	this->_nick = other.getNick();
	this->_client_addr = other.getClientAddr();
}
User &User::operator= (const User &other)
{
	std::cout << YELLOW << ": Called assignment operator (USER) " << DEFCOL;

	this->_name = other.getName();
	this->_nick = other.getNick();
	this->_client_addr = other.getClientAddr();

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

std::string	User::getName(void) const { return (this->_name);}
std::string	User::getNick(void) const { return (this->_nick);}
void User::setName(std::string name) { this->_name = name; }
void User::setNick(std::string nick) { this->_nick = nick; }
void User::setUsername(std::string userName) { this->_userName = userName; }
void User::setMode(std::string mode) { this->_mode = mode; }
sockaddr_in	User::getClientAddr(void) const { return (this->_client_addr);}

// 0================ OTHER FUNCTIONS ================0

void	User::parseUserInfo(std::string *args){
	this->setUsername(args[1]);
	this->setMode(args[2]);
	this->setName(args[4]);
}