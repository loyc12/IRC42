#include "Channel.hpp"

// 0================ BASE FUNCTIONS ================0

Channel::Channel(std::string nameChan): _nameChan(nameChan){ return; }
Channel::~Channel(void) { return; }

// 0================ GETTERS / SETTERS ================0

std::string const & Channel::getNameChan(void) const { return (this->_nameChan); }
std::string const & Channel::getAdmin(void) const { return (this->_admin); }
std::string const & Channel::getPass(void) const { return (this->_password); }
void				Channel::setAdmin(std::string const &admin) { this->_admin = admin; }

// 0================ OTHER FUNCTIONS ================0

void	Channel::joinChan(Server *server, User *user, int fd){
	//setup of msg to send when join channel
	std::ostringstream joinMsg;
	joinMsg << ":" << user->getNick() << "!" << user->getUsername() << "@" << server->getNameServer() << " JOIN " << this->getNameChan() << "\r\n";
	std::string joinChan = joinMsg.str();
	//letting everyone knows what happens
	send(fd, joinChan.c_str(), joinChan.size(), 0);

}
