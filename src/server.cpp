#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
Server::Server() : _port(6667) {
	std::cout << ": Called default constructor (SERVER) " << std::endl;
}

//public
Server::Server(int port) : _port(port) {
	std::cout << ": Called parameterized constructor (SERVER) " << std::endl;
}
Server::Server(const Server &other)
{
	std::cout << ": Called copy constructor (SERVER) ";
	this->_port = other.getPort();
}
Server &Server::operator= (const Server &other)
{
	std::cout << ": Called assignment operator (SERVER) ";
	this->_port = other.getPort();
	return *this;
}
Server::~Server()
{
	std::cout << ": Called destructor (SERVER) ";
	//TODO : FREE
}
std::ostream &operator<< (std::ostream &out, const Server &rhs)
{
	out << "IRC Server port : " << rhs.getPort();
	return (out);
}

// 0================ Getters & Setters ================0

const int & Server::getPort(void) const { return (this->_port);}

// 0================ OTHER FUNCTIONS ================0

