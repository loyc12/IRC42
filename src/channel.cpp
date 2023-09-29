#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

Channel::Channel(std::string nameChan): _nameChan(nameChan){ return; }
Channel::~Channel(void) { return; }

// 0================ GETTERS / SETTERS ================0

std::string const & Channel::getNameChan(void) const { return (this->_nameChan); }
std::string const & Channel::getAdmin(void) const { return (this->_admin); }
std::string const & Channel::getPass(void) const { return (this->_password); }
void				Channel::setAdmin(std::string const &admin) { this->_admin = admin; }
void				Channel::setNameChan(std::string const &chan) { this->_nameChan = chan; }

// 0================ OTHER FUNCTIONS ================0

//The channel is created implicitly when the first client joins it, and the channel ceases to exist when the last client leaves it.
/*
:test!test@127.0.0.1 JOIN #

*/

void	Channel::joinChan(User *user, int fd){
	std::cout << "JOINCHAN()" << std::endl;
	std::ostringstream joinMsg;
	joinMsg << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " JOIN " << this->getNameChan() << "\n";
	std::cout << joinMsg.str() << std::endl;
	std::string joinChan = joinMsg.str();
	//letting everyone knows what happens
	if (send(fd, joinChan.c_str(), joinChan.size(), 0) < 0)
		throw std::invalid_argument(" > Error at joinChan() ");

}
