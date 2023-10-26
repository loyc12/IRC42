#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

User::User(sockaddr_in clientAddr) : _fd(0), _name("*"), _userName("*"), _nick("*"), _hostName("*"), _clientAddr(clientAddr), wasWelcomed(false), lastMsg("")
														{ debugPrint(YELLOW, CONSTR_USER); }
User::~User()											{ debugPrint(YELLOW, DEST_USER); }

// 0============== GETTERS - SETTERS ==============0

int			User::getFD(void) const						{ return (this->_fd); }
std::string	User::getName(void) const 					{ return (this->_name);}
std::string	User::getNick(void) const 					{ return (this->_nick);}
std::string User::getUsername(void) const 				{ return (this->_userName);}
std::string User::getHostname(void) const 				{ return (this->_hostName); }
sockaddr_in	User::getClientAddr(void) const 			{ return (this->_clientAddr); }

void		User::setFD(int fd)							{ this->_fd = fd; }
void 		User::setName(std::string name) 			{ this->_name = name; }
void 		User::setNick(std::string nick) 			{ this->_nick = nick; }
void 		User::setUsername(std::string userName) 	{ this->_userName = userName; }
void 		User::setHostname(std::string hostName) 	{ this->_hostName = hostName; }

// 0================ OTHER FUNCTIONS ================0

void	User::setUserInfo(std::vector<std::string> args)
{
	this->setUsername(args[1]);
	this->setHostname(inet_ntoa(this->_clientAddr.sin_addr));
	this->setName(args[4]);
}
