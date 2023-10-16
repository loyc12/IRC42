#include "IRC.hpp"



bool	Server::isUserInChan(User *user, Channel *chan)
{
	if (chan->hasMember(user))
	{
		replyTo(REQUEST, user, user, ERR_ALREADYREGISTRED, "Already registered");
	 	return (true);
	}
	return (false);
}



bool	Server::checkInvitePerm(User *user, Channel *chan)
{
//	Check if the mode of the chan is in Invitation only
	if (chan->getInviteFlag())
	{
		replyTo(REQUEST, user, user, ERR_INVITEONLYCHAN, "The channel is invite only");
		return (false);
	}
	return (true);
}



bool	Server::checkPass(User *user, Channel *chan, std::string pass)
{
//	Check password of channel (if they are one)
	if (chan->getPass().length() != 0)
	{
//		Check password provided, return error if no or bad password
		if (pass.length() == 0)
		{
			replyTo(REQUEST, user, user, ERR_NEEDMOREPARAMS, "No password provided");
			return (false);
		}
		else if (pass.compare(chan->getPass()) != 0)
		{
			replyTo(REQUEST, user, user, ERR_PASSWDMISMATCH, "Invalid password");
			return (false);
		}
	}
	return (true);
}



bool	Server::checkMaxMbr(User *user, Channel *chan)
{
//	Check if we can add a member to the channel list
	if (chan->getMaxMbrCnt() > 0 && chan->getMemberCnt() >= chan->getMaxMbrCnt())
	{
		replyTo(REQUEST, user, user, ERR_CHANNELISFULL, "Cannot join channel");
		return (false);
	}
	return (true);
}



Channel	*Server::findChannel(std::string str)
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(str);

	std::cerr << "findChannel()" << std::endl; //										DEBUG
	if (it != this->_chanContainer.end())
	{
		std::cerr << "channel found" << std::endl; //									DEBUG
		return (it->second);
	}
	else
		return (NULL);
}



User	*Server::findUser(std::string str)
{
	std::cerr << "findUser()" << std::endl; //											DEBUG
	for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getNick().compare(str) == 0)
		{
			std::cerr << "user found : " << it->second->getNick() << std::endl; //		DEBUG
			return (it->second);
		}
	}
	return (NULL);
}
