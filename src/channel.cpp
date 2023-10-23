#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)		{
	debugPrint(YELLOW, CONSTR_CHAN);
	this->_topic = "None";
	this->_isInviteOnly = 0; }
Channel::~Channel(void) 										{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const			{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 			{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 				{ return (this->_password); }
std::string const & Channel::getTopic(void) const				{ return (this->_topic); }
int 		const & Channel::getMaxMbrCnt(void) const			{ return (this->_maxMemberCount); }
int 				Channel::getMemberCnt(void) const			{ return (this->_chanMembers.size()); }
bool 		const & Channel::getInviteFlag(void)const			{ return (this->_isInviteOnly); }

void	Channel::setChanName(std::string const &chan)			{ this->_chanName = chan; }
void	Channel::setAdminName(std::string const &admin) 		{ this->_adminName = admin; }
void	Channel::setPass(std::string const &password)			{ this->_password = password; }
void	Channel::setTopic(std::string const &topic)				{ this->_topic = topic; }
void	Channel::setMaxMemberCount(int const &count)			{ this->_maxMemberCount = count; }
void	Channel::setInviteFlag(bool const &boolean)				{ this->_isInviteOnly = boolean; }

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
//	return true if nickname already used
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
//	return true if user* already in list
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


void	Channel::addChanOp(User *user)
{
	this->_chanOps.push_back(user);
}


void	Channel::addMember(User *user)
{
//	adds user to userlist if it is not in already
	if (!hasMember(user))
	{
		this->_chanMembers.push_back(user);
//		sendMemberList();
	}
//	else
//		error message
}



void	Channel::removeMember(User *user) //				NOTE : when deleting a client, remove it from all channel first
{
	if (hasMember(user))
	{
		for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
		{
			if (isSameUser(user, *it))
			{
				this->_chanMembers.erase(it);
				return ;
			}
		}
	}
//	else
//		error message
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
	message << ": 331 " << user->getUsername() << " " << this->getChanName() << " :" << this->getTopic() << "\r\n";
	message << ": 353 " << user->getUsername() << " = " << this->getChanName() << " :" << memberList << "\r\n";
	message << ": 366 " << user->getUsername() << " " << this->getChanName() << " :" << "End of NAMES list" << "\r\n";

	this->sendToChan(user, message.str(), true);
}



//		SENDS A MESSAGE TO EVERYONE IN THE SERVER
void	Channel::sendToChan(User *sender, std::string message, bool sendToSender) 
{
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		std::ostringstream debug; //													DEBUG
		debug << "OUTGOING CHAN_MSG TO (" << (*it)->getFD() << ") :\n" << message; //	DEBUG
		debugPrint(MAGENTA, debug.str()); //												DEBUG

		//	Checks if we need to skip the sender or not
		if (sendToSender || !isSameUser((*it), sender))
		{
			if (send((*it)->getFD(), message.c_str(), message.size(), 0) < 0)
				throw std::invalid_argument(" > Error at sendToChan() ");
		}
		else
			std::cerr << "skiping the sender" << std::endl; //							DEBUG
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
