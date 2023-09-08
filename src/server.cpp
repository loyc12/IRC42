#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
Server::Server() : _port(6667), _password("1234"){
	std::cout << YELLOW << ": Called default constructor (SERVER) " << DEFCOL;
}

//public
Server::Server(int port) : _port(port), _password("1234") {
	std::cout << YELLOW << ": Called parameterized constructor (SERVER) " << DEFCOL;
}
Server::Server(const Server &other)
{
	std::cout << YELLOW << ": Called copy constructor (SERVER) " << DEFCOL;
	this->_port = other.getPort();
}
Server &Server::operator= (const Server &other)
{
	std::cout << YELLOW << ": Called assignment operator (SERVER) " << DEFCOL;
	this->_port = other.getPort();
	return *this;
}
Server::~Server()
{
	std::cout << YELLOW << ": Called destructor (SERVER) " << DEFCOL;
	//TODO : FREE
}
std::ostream &operator<< (std::ostream &out, const Server &rhs)
{
	out << "IRC Server port : " << rhs.getPort();
	return (out);
}

// 0================ GETTERS / SETTERS ================0

const int & Server::getPort(void) const { return (this->_port);}

const std::string & Server::getPass(void) const { return (this->_password);}

// 0================ OTHER FUNCTIONS ================0

