#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0

User::User(sockaddr_in clientAddr) : _fd(0), _loginPass(0), _loginNick(0), _loginUser(0), _wasWelcomed(false), _lastMsg(""), _name("*"), _userName("*"), _nick("*"), _hostName("*"), _clientAddr(clientAddr)
														{ debugPrint(YELLOW, CONSTR_USER); }
User::~User()											{ debugPrint(YELLOW, DEST_USER); }

// 0============== GETTERS - SETTERS ==============0

int			User::getFD(void) const						{ return (this->_fd); }
//int			User::getLoginStep(void) const				{ return (this->_loginStep); }
bool		User::wasWelcomed(void) const				{ return (this->_wasWelcomed); }
std::string	User::getLastMsg(void) const				{ return (this->_lastMsg); }
std::string	User::getName(void) const 					{ return (this->_name);}
std::string	User::getNick(void) const 					{ return (this->_nick);}
std::string User::getUsername(void) const 				{ return (this->_userName);}
std::string User::getHostname(void) const 				{ return (this->_hostName); }

sockaddr_in	User::getClientAddr(void) const 			{ return (this->_clientAddr); }

void		User::setFD(int fd)							{ this->_fd = fd; }
void		User::setWelcome(bool boolean)				{ this->_wasWelcomed = boolean; }
void		User::setLastMsg(std::string msg)
{
	if (msg.empty())
	{
		if (!this->_lastMsg.empty())
			this->_lastMsg.clear();
	}
	else
		this->_lastMsg = msg;
}
void 		User::setName(std::string name) 			{ this->_name = name; }
void 		User::setNick(std::string nick)
{
	this->_nick = nick;
}
void 		User::setUsername(std::string userName)
{
	this->_userName = userName;
}
void 		User::setHostname(std::string hostName) 	{ this->_hostName = hostName; }

// 0================ OTHER FUNCTIONS ================0

void	User::setUserInfo(std::vector<std::string> args)
{
	this->setUsername(args[1]);
	this->setHostname(inet_ntoa(this->_clientAddr.sin_addr));
	this->setName(args[4]);
}

void	User::addLoginStep(std::string step)
{
	if 		(step.compare("PASS") == 0) this->_loginPass = true;
	else if	(step.compare("NICK") == 0) this->_loginNick = true;
	else if	(step.compare("USER") == 0) this->_loginUser = true;
	//else	throw std::invalid_argument(" > Error at User::addLoginStep() : invalid step");
}

bool	User::isLoggedIn(void)
{
	if (this->_loginPass && this->_loginNick && this->_loginUser)
		return(true);
	return (false);
}

