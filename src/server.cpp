#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

Server::Server()
{
	std::cout << ": Called default constructor (SERVER) ";
	this->port = 6667;
	this->port = 1234;
}
Server::Server(int _port, int _pass)
{
	std::cout << ": Called parameterized constructor (SERVER) ";

	this->port = _port;
	this->pass = _pass;
}
Server::Server(const Server &other)
{
	std::cout << ": Called copy constructor (SERVER) ";

	this->port = other.getPort();
	this->pass = other.getPass();
}
Server &Server::operator= (const Server &other)
{
	std::cout << ": Called assignment operator (SERVER) ";

	this->port = other.getPort();
	this->pass = other.getPass();

	return *this;
}
Server::~Server()
{
	std::cout << ": Called destructor (SERVER) ";

	// TODO
}
std::ostream &operator<< (std::ostream &out, const Server &rhs)
{
	out << "IRC Server [ port : " << rhs.getPort() << " | pass : " << rhs.getPass() << " ]";

	// TODO

	return (out);
}

// 0================ OTHER FUNCTIONS ================0

int	Server::getPort(void) const
{
	return (this->port);
}
int	Server::getPass(void) const
{
	return (this->pass);
}

// 0================ OTHER FUNCTIONS ================0
