#include "IRC.hpp"

//	0================ BASE FUNCTIONS ================0

Channel::Channel(std::string chanName): _chanName(chanName)		{ debugPrint(YELLOW, CONSTR_CHAN); }
Channel::~Channel(void) 										{ debugPrint(YELLOW, DEST_CHAN); }

//	0================ GETTERS - SETTERS ================0

std::string const & Channel::getChanName(void) const			{ return (this->_chanName); }
std::string const & Channel::getAdminName(void) const 			{ return (this->_adminName); }
std::string const & Channel::getPass(void) const 				{ return (this->_password); }
int const & Channel::getMaxMbrCnt(void) const					{ return (this->_maxMemberCount); }
int Channel::getMemberCnt(void) const							{ return (this->_chanMembers.size()); }
bool const & Channel::getInviteOnly(void)const					{ return (this->_isInviteOnly); }

void	Channel::setChanName(std::string const &chan)			{ this->_chanName = chan; }
void	Channel::setAdminName(std::string const &admin) 		{ this->_adminName = admin; }
void	Channel::setPass(std::string const &password)			{ this->_password = password; }
void	Channel::setMaxMemberCount(int const &count)			{ this->_maxMemberCount = count; }
void	Channel::setInviteOnly(bool const &boolean)				{ this->_isInviteOnly = boolean; }

//	0================ OTHER FUNCTIONS ================0

bool Channel::isSameUser(User* user1, User* user2)
{
	if ((void *)user1 == (void *)user2) //									WARNING : does that even work?
	{
		std::cerr << std::endl << "HALLO" << std::endl;
		return (true);
	}
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
/*
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); ++it)
	{
		if (isSameUser(*it, user))
			return (true);
	}
	return (false)
*/
}

void	Channel::addMember(User *user)
{
	if (!hasMember(user))
	{
//		add user to list here
		this->_chanMembers.push_back(user);
		//return ;
	}
//	error : already present ??? **I say, if already there, do nothing. Just print the list of members
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		std::cout << (*it)->getNick() << std::endl; //							DEBUG; eventually needs to be send to the newly join members
	}

}


void	Channel::removeMember(User *user) //					NOTE : when deleting a client, remove it from all channel first
{
	if (hasMember(user))
	{
		for (int i = 0; i < this->getMemberCnt(); i++)
		{
			if (isSameUser(user, _chanMembers[i]))
				this->_chanMembers.erase(this->_chanMembers.begin() + i);
		}
		/*
//		remove user from list here
		for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); ++it)
		{
			if (compareUsers(*it, user))
			{
				delete *it;
				this->_chanMembers.erase(it);
			}
		}
		*/
	}
	for (std::vector<User*>::iterator it = this->_chanMembers.begin(); it != this->_chanMembers.end(); it++)
	{
		std::cout << (*it)->getNick() << std::endl; //							DEBUG;
	}
	//else send error message (?)
}

User 	*Channel::getMember(int i)
{
	if (i < getMemberCnt())
		return (_chanMembers[i]);
	else
		throw std::invalid_argument(" > Error at Channel::getMember() ");
}