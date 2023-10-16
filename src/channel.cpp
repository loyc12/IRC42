#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)		{
	debugPrint(YELLOW, CONSTR_CHAN);
	this->_topic = "No topic set.";
	this->_isInviteOnly = 0; }
Channel::~Channel(void) 										{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const			{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 			{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 				{ return (this->_password); }
std::string const & Channel::getTopic(void) const				{ return (this->_topic); }
int const & 		Channel::getMaxMbrCnt(void) const			{ return (this->_maxMemberCount); }
int 				Channel::getMemberCnt(void) const			{ return (this->_chanMembers.size()); }
bool const & 		Channel::getInviteFlag(void)const			{ return (this->_isInviteOnly); }

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

bool	Channel::hasMember(User *user)
{
//	return true if user* already in list
	for (int i = 0; i < this->getMemberCnt(); i++)
	{
		if (isSameUser(user, _chanMembers[i]))
			return (true);
	}
	return (false);
}

void	Channel::addChanOps(User *user)
{
	this->_chanOps.push_back(user);
}

void	Channel::addMember(User *user)
{
//	adds user to userlist if it is not in already
	if (!hasMember(user))
		this->_chanMembers.push_back(user);
//	else
//		error message
}

void	Channel::removeMember(User *user) //				NOTE : when deleting a client, remove it from all channel first
{
	if (hasMember(user))
	{
		for (int i = 0; i < this->getMemberCnt(); i++)
		{
			if (isSameUser(user, _chanMembers[i]))
				this->_chanMembers.erase(this->_chanMembers.begin() + i);
		}
	}
//	else
//		error message
//	{
//		std::cout << (*it)->getNick() << std::endl; //							DEBUG;
//	}
}

User 	*Channel::getMember(int i)
{
	if (i < getMemberCnt())
		return (_chanMembers[i]);
	else
		throw std::invalid_argument(" > Error at Channel::getMember() ");
}


void	Channel::replyToChan(User* user, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

	message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << code << " " << input << "\r\n";

	std::string listMembers;
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		listMembers += (*it)->getNick() + " ";
	}

//not send to people; info print in terminal
	message << ":" << " 331 " << user->getUsername() << " " << this->getChanName() << " :" << this->getTopic() << "\r\n";
	message << ":" << " 353 " << user->getUsername() << " = " << this->getChanName() << " :" << listMembers << "\r\n";
	message << ":" << " 366 " << user->getUsername() << " " << this->getChanName() << " :" << "End of NAMES list" << "\r\n";
	result = message.str();
	std::ostringstream debug; //												DEBUG
	debug << "OUTGOING C_MSG TO : (" << user->getFD() << ")\t| " << result; //	DEBUG
	debugPrint(GREEN, debug.str()); //											DEBUG

	if (send(user->getFD(), result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at replyTo() ");
}