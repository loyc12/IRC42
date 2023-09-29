#include "IRC.hpp"

void	User::debugPrint(std::string color, std::string message){ std::cout << color << message << DEFCOL;}
User::User(sockaddr_in client_addr) : _client_addr(client_addr) { debugPrint(YELLOW, CONSTR_USER); }
User::~User()													{ debugPrint(YELLOW, DEST_USER); }

std::string	User::getName(void) const 							{ return (this->_name);}
std::string	User::getNick(void) const 							{ return (this->_nick);}
std::string User::getUsername(void) const 						{ return (this->_userName);}
std::string User::getMode(void) const 							{ return (this->_mode);}
std::string User::getHostname(void) const 						{ return (this->_hostname); }
void User::setName(std::string name) 							{ this->_name = name; }
void User::setNick(std::string nick) 							{ this->_nick = nick; }
void User::setUsername(std::string userName) 					{ this->_userName = userName; }
void User::setMode(std::string mode) 							{ this->_mode = mode; }
void User::setHostname(std::string hostname) 					{ this->_hostname = hostname; }
sockaddr_in	User::getClientAddr(void) const 					{ return (this->_client_addr); }





// 0================ OTHER FUNCTIONS ================0

void	User::parseUserInfo(std::string *args)
{
	std::cout << "here\n" << std::endl;
	this->setUsername(args[1]);
	this->setMode(args[2]);
	this->setHostname(inet_ntoa(this->_client_addr.sin_addr));
	this->setName(args[4]);
}
