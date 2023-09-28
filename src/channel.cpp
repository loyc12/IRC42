#include "Channel.hpp"

// 0================ BASE FUNCTIONS ================0

Channel::Channel(std::string nameChan): _nameChan(nameChan){  debugPrint(YELLOW, ": Called constructor (CHANNEL) "); }
Channel::~Channel(void) {  debugPrint(YELLOW, ": Called destructor (CHANNEL) ");  }

// 0================ GETTERS / SETTERS ================0

std::string const & Channel::getNameChan(void) const { return (this->_nameChan); }
std::string const & Channel::getAdmin(void) const { return (this->_admin); }
std::string const & Channel::getPass(void) const { return (this->_password); }
void				Channel::setAdmin(std::string const &admin) { this->_admin = admin; }
void				Channel::setNameChan(std::string const &chan) { this->_nameChan = chan; }

// 0================ OTHER FUNCTIONS ================0

void	Channel::joinChan(User *user){
	//setup of msg to send when join channel
	std::ostringstream joinMsg;
	joinMsg << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " JOIN " << this->getNameChan() << "\n";
	std::cout << joinMsg.str() << std::endl;
	std::string joinChan = joinMsg.str();

	//letting everyone knows what happens
	send(user->getFD(), joinChan.c_str(), joinChan.size(), 0); //			USE sendToClient() INSTEAD ???

}
