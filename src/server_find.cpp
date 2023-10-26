#include "IRC.hpp"

bool	Server::isUserInChan(User *user, Channel *chan)
{
	if (chan->hasMember(user))
	{
	 	sendToUser(user, makeUserMsg(user, "ERR_ALREADYREGISTRED", "Already registered"));
		return (true);
	}
	return (false);
}



bool	Server::checkInvitePerm(User *user, Channel *chan)
{
//	Check if the mode of the chan is in Invitation only
	if (chan->getInviteFlag())
	{
		sendToUser(user, makeUserMsg(user, ERR_INVITEONLYCHAN, "The channel is invite only"));
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
			sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "No password provided"));
			return (false);
		}
		else if (pass.compare(chan->getPass()) != 0)
		{
			sendToUser(user, makeUserMsg(user, ERR_PASSWDMISMATCH, "Invalid password"));
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
		sendToUser(user, makeUserMsg(user, ERR_CHANNELISFULL, "Cannot join channel (full)"));
		return (false);
	}
	return (true);
}



bool	Server::isNickValid(User *user, std::string nickname)
{
	for (int i = 0; i < (int)nickname.length(); i++)
	{
		if (std::isalnum(nickname[i]) == 0 && nickname[i] != '_')
		{
			sendToUser(user, makeUserMsg(user, ERR_ERRONEUSNICKNAME, "Invalid nickname (" + nickname + " has non alphanumeric characters)"));
			return (false);
		}
	}
	for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getNick().compare(nickname) == 0)
		{
			sendToUser(user, makeUserMsg(user, ERR_NICKNAMEINUSE, "Invalid nickname (" + nickname + " is already taken)"));
			return (false);
		}
	}
	return (true);
}

bool	Server::isMsgEnd(std::string str)
{
	(void)str;
	return (true); //								TODO : IMPLEMENT ME
	/*
	if (str.length() > 0 && str[str.length() - 1] == '\n') //&& str[str.length() - 2] == '\r')
		return (true);

	std::cerr << "message is not completed yet" << std::endl; //							DEBUG
	return (false);
	*/
}



Channel	*Server::findChannel(std::string str)
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(str);

	if (it != this->_chanContainer.end())
	{
		return (it->second);
	}
	return (NULL);
}



User	*Server::findUser(std::string str)
{
	for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getNick().compare(str) == 0)
		{
			return (it->second);
		}
	}
	return (NULL);
}
