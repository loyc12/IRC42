#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
Server::Server() : _port(6667), _newSocketFd(0), _baseSocketFd(0) {
	std::cout << ": Called default constructor (SERVER) " << std::endl;
}
//public
Server::Server(int port) : _port(port), _newSocketFd(0), _baseSocketFd(0) {
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
const int & Server::getBaseSocket(void) const { return (this->_baseSocketFd);}
const int & Server::getNewSocket(void) const { return (this->_newSocketFd);}

void Server::setBaseSocket(int baseSocket) { this->_baseSocketFd = baseSocket; return ;}
void Server::setNewSocket(int newSocket) { this->_newSocketFd = newSocket; return ;}

// 0================ OTHER FUNCTIONS ================0

void Server::initSocket(void) {

	// setBaseSocket(socket(AF_INET, SOCK_STREAM, 0));
	// if (getBaseSocket() < 0)
	// 	throw std::invalid_argument(" > Error at socket(): ");
	// else
	// 	std::cout << "Socket() is OK!" << std::endl;

	// //	Makes it so that read/write call to sockets return -1 if blocking
	// if (fcntl(getBaseSocket(), F_SETFL, O_NONBLOCK))
	// 	throw std::invalid_argument(" > Error at fcntl() : ");
	// else
	// 	std::cout << "fcntl() is OK!" << std::endl;
}

// void Server::initBind(struct sockaddr_in *server_addr) {

// 	bzero((char *) &*server_addr, sizeof(*server_addr));
// 	server_addr->sin_family = AF_INET; //			bind call
//     server_addr->sin_addr.s_addr = INADDR_ANY; //	host ip adress **INADDR_ANY go get localhost
//     server_addr->sin_port = htons(getPort()); //			conversion to network byte order (Ip adress)

// 	if (bind(getBaseSocket(), (struct sockaddr *) &*server_addr, sizeof(*server_addr)) < 0)
// 		throw std::invalid_argument(" > Error at bind(): ");
// 	else
// 		std::cout << "Bind() is OK!" << std::endl;

// }

//void Server::acceptRequest(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr) {

		// int newSocket;
		// socklen_t client_len = sizeof(*client_addr);
		// newSocket = accept(getBaseSocket(), (struct sockaddr *) &*client_addr, &client_len);
		// setNewSocket(newSocket);
		// if (getNewSocket() < 0)
		// {
		// 	std::cerr << std::cout << std::strerror(errno) << std::endl;
		// }
		// else if (getNewSocket())
		// {
		// 	std::cout << "\nCONNECTED\nCLIENT FROM NETWORK :\t" << inet_ntoa(client_addr->sin_addr) << "\nNET-TO-HOST PORT :\t" << ntohs(client_addr->sin_port) << std::endl;
		// 	std::cout << "\nSERVER IP (local): \t" << inet_ntoa(server_addr->sin_addr) << "\nHOST-TO-NET PORT :\t" << ntohs(server_addr->sin_port) << std::endl;
		// }
//}
