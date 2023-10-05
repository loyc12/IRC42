#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)			{ debugPrint(YELLOW, CONSTR_CHAN); }
Channel::~Channel(void) 											{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const				{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 				{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 					{ return (this->_password); }
int const & Channel::getMaxMemberCount(void) const					{ return (this->_maxMemberCount); }
bool const & Channel::getInviteOnly(void)const						{ return (this->_isInviteOnly); }

void	Channel::setChanName(std::string const &chan)				{ this->_chanName = chan; }
void	Channel::setAdminName(std::string const &admin) 			{ this->_adminName = admin; }
void	Channel::setPass(std::string const &password)				{ this->_password = password; }
void	Channel::setMaxMemberCount(int const &count)				{ this->_maxMemberCount = count; }
void	Channel::setInviteOnly(bool const &boolean)					{ this->_isInviteOnly = boolean; }
//	0================ OTHER FUNCTIONS ================0
