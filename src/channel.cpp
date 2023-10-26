#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)		{
	debugPrint(YELLOW, CONSTR_CHAN);
	this->_topic = "No topic is set";
	this->_isInviteOnly = 0;
	this->_canTopic = 0;
	this->_maxMemberCount = 0;}
Channel::~Channel(void) 										{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const			{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 			{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 				{ return (this->_password); }
std::string const & Channel::getTopic(void) const				{ return (this->_topic); }
int 		const & Channel::getMaxMbrCnt(void) const			{ return (this->_maxMemberCount); }
bool 		const & Channel::getInviteFlag(void)const			{ return (this->_isInviteOnly); }
bool		const & Channel::getTopicFlag(void) const			{ return (this->_canTopic); }
int 				Channel::getMemberCnt(void) const			{ return (this->_chanMembers.size()); }
int 				Channel::getOpCnt(void) const				{ return (this->_chanOps.size()); }


void	Channel::setChanName(std::string const &chan)			{ this->_chanName = chan; }
void	Channel::setAdminName(std::string const &admin) 		{ this->_adminName = admin; }
void	Channel::setPass(std::string const &password)			{ this->_password = password; }
void	Channel::setTopic(std::string const &topic)				{ this->_topic = topic; }
void	Channel::setMaxMemberCount(int const &count)			{ this->_maxMemberCount = count; }
void	Channel::setInviteFlag(bool const &boolean)				{ this->_isInviteOnly = boolean; }
void	Channel::setTopicFlag(bool const &boolean)				{ this->_canTopic = boolean; }


//	0================ OTHER FUNCTIONS ================0

bool Channel::isSameUser(User* user1, User* user2)
{
	if ((void *)user1 == (void *)user2)
		return (true);
	return (false);
}

bool	Channel::isChanOp(User *user)
{
	for (std::vector<User*>::iterator it = this->_chanOps.begin(); it != this->_chanOps.end(); it++)
	{
		if (*it == user)
			return (true);
	}
	return (false);
}

bool	Channel::hasSameNick(User *user)
{
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		if ((*it)->getNick().compare(user->getNick()) == 0)
		{
			sendToUser(user, makeUserMsg(user, ERR_NICKNAMEINUSE, "Nickname already used"));
			return (true);
		}
	}
	return (false);
}

bool	Channel::hasMember(User *user)
{
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		if (isSameUser(user, *it))
			return (true);
	}
	return (false);
}

bool	Channel::hasChanOp(User *user)
{
	for (std::vector<User*>::iterator it = this->_chanOps.begin(); it != this->_chanOps.end(); it++)
	{
		if (isSameUser(user, *it))
			return (true);
	}
	return (false);
}

// ADDS USER TO USERLIST IF IT'S NOT IN ALREADY
void	Channel::addMember(User *user)
{
	if (!hasMember(user))
		this->_chanMembers.push_back(user);
	if (getOpCnt() < 1)
		resetOpp();
}

void	Channel::addChanOp(User *user)
{
	if (hasMember(user) && !hasChanOp(user))
		this->_chanOps.push_back(user);
}

void	Channel::removeMember(User *user)
{
	if (hasMember(user))
	{
		for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
		{
			if (isSameUser(user, *it))
			{
				User *tmp = *it;
				this->_chanMembers.erase(it);
				if (hasChanOp(tmp))
					removeChanOp(tmp);
				return ;
			}
		}
	}
}

void	Channel::removeChanOp(User *user)
{
	for (std::vector<User*>::iterator it = this->_chanOps.begin(); it != this->_chanOps.end(); it++)
	{
		if (isSameUser(user, *it))
		{
			this->_chanOps.erase(it);
			if (getOpCnt() < 1)
				resetOpp();
			return ;
		}
	}
}

void	Channel::resetOpp(void)
{
	addChanOp(*(this->_chanMembers.begin()));
}

User 	*Channel::getMember(int id)
{
	int	j = 0;
	if (id < getMemberCnt())
	{
		for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
		{
			if (j++ == id)
				return (*it);
		}
	}
	throw std::invalid_argument(" > Error at Channel::getMember() : invalid member id (too big)");
}


//		UPDATES THE MEMBER LIST AND SENDS IT TO ALL CHANNEL MEMBERS
void	Channel::updateMemberList(User *user)
{
	std::string memberList;
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		memberList += (*it)->getNick() + " ";
	}
	std::ostringstream message;

	message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << "\r\n";
	message << ": " << RPL_TOPIC << " " << user->getUsername() << " " << this->getChanName() << " :" << this->getTopic() << "\r\n";
	message << ": " << RPL_NAMREPLY << " " << user->getUsername() << " = " << this->getChanName() << " :" << memberList << "\r\n";
	message << ": " << RPL_ENDOFNAMES << " " << user->getUsername() << " " << this->getChanName() << " :" << "End of NAMES list" << "\r\n";

	this->sendToChan(user, message.str(), true);
}

//		SENDS A MESSAGE TO EVERYONE IN THE SERVER
void	Channel::sendToChan(User *sender, std::string message, bool sendToSender)
{
	printChanOps(); //																	DEBUG

	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		std::ostringstream debug; //													DEBUG
		debug << "OUTGOING CHAN_MSG TO (" << (*it)->getNick() << ") :\n" << message; //	DEBUG
		debugPrint(MAGENTA, debug.str()); //											DEBUG

		//	Checks if we need to skip the sender or not
		if (sendToSender || !isSameUser((*it), sender))
		{
			if (send((*it)->getFD(), message.c_str(), message.size(), 0) < 0)
				throw std::invalid_argument(" > Error at sendToChan() ");
		}
	}
}


//		DEBUG FUNCTION
void	Channel::printMembers(void)
{
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		std::cerr << (*it)->getNick() << ", ";
	}
	std::cerr << std::endl;
}

//		DEBUG FUNCTION
void	Channel::printChanOps(void)
{
	for (std::vector<User*>::iterator it = this->_chanOps.begin(); it != this->_chanOps.end(); it++)
	{
		std::cerr << (*it)->getNick() << ", ";
	}
	std::cerr << std::endl;
}
