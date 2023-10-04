#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)			{ debugPrint(YELLOW, CONSTR_CHAN); }
Channel::~Channel(void) 											{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const				{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 				{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 					{ return (this->_password); }

void				Channel::setChanName(std::string const &chan)	{ this->_chanName = chan; }
void				Channel::setAdminName(std::string const &admin) { this->_adminName = admin; }

//	0================ OTHER FUNCTIONS ================0
