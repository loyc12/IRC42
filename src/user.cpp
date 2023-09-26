#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
User::User() { std::cout << YELLOW << ": Called default constructor (USER) " << DEFCOL; }

// User::User(const User &other)
// {
// 	std::cout << YELLOW << ": Called copy constructor (USER) " << DEFCOL;
// 	this->_name = other.getName();
// 	this->_nick = other.getNick();
// 	this->_client_addr = other.getClientAddr();
// }

// User &User::operator= (const User &other)
// {
// 	std::cout << YELLOW << ": Called assignment operator (USER) " << DEFCOL;

// 	this->_name = other.getName();
// 	this->_nick = other.getNick();
// 	this->_client_addr = other.getClientAddr();

// 	return *this;
// }

// std::ostream &operator<< (std::ostream &out, const User &rhs)
// {
// 	out << "IRC User [ name : " << rhs.getName() << " | nick : " << rhs.getNick() << " ]";
// 	return (out);
// }

//new Client
User::User(sockaddr_in client_addr) : _client_addr(client_addr) {
	std::cout << YELLOW << ": Called constructor (USER) " << DEFCOL; }
// Delete Client
User::~User(){ std::cout << YELLOW << ": Called destructor (USER) " << DEFCOL;}


// 0================ GETTERS / SETTERS ================0

std::string	User::getName(void) const { return (this->_name);}
std::string	User::getNick(void) const { return (this->_nick);}
void User::setName(std::string name) { this->_name = name; }
void User::setNick(std::string nick) { this->_nick = nick; }
sockaddr_in	User::getClientAddr(void) const { return (this->_client_addr);}

// 0================ OTHER FUNCTIONS ================0
