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
	//send msg when added to channel
	(void)user;
	//std::cout << ":" << server->getNameServer() << " JOIN " << this->getNameChan() << std::endl;
	std::string joinMSG = ":" + server->getNameServer() + " JOIN " + this->getNameChan() + "\r\n";
	send(fd, joinMSG.c_str(), joinMSG.size(), 0);
	std::cout << "who is the admin" << this->getAdmin() << std::endl;
}
