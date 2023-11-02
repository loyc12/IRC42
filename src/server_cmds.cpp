#include "IRC.hpp"

// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, std::vector<std::string> args)
{
	if (user->wasWelcomed())
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTERED, "Already registered"));
	else if (args[1].compare(std::to_string(this->getPass())) != 0)
	{
		sendToUser(user, makeUserMsg(user, ERR_PASSWDMISMATCH, "Invalid password"));

		//	deletes client if they fail to use the proper password on login
		deleteClient(user->getFD());
	}
	else
		user->addLoginStep("PASS");
	return (0);
}

int	Server::storeNickname(User *user, std::vector<std::string> args)
{
	if (isNickValid(user, args[1])) //	NOTE : this send its own error messages
	{
		std::string oldname = user->getNick();
		user->setNick(args[1]);
		user->addLoginStep("NICK");

		if (user->wasWelcomed())
		{
			// send to EVERYONE on the serv that user changed name
			(void)":oldname NICK newname"; // TODO use a new "sendToServ()" function that sends this to everyone
			std::string msg = oldname + " : " + args[1];
			sendToServ(user, makeUserMsg(user, "NICK", msg));
		}
	}
	//	deletes client if they fail to set their nickname properly on login
	else if (!user->wasWelcomed())
		deleteClient(user->getFD());
	return (0);
}

int	Server::storeUserInfo(User *user, std::vector<std::string> args)
{
	if (user->wasWelcomed())
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTERED, "Already registered"));
	else if (args.size() < 5)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (isInfoValid(user, args))
	{
		user->setUserInfo(args);
		user->addLoginStep("USER");
	}
	return (0);
}

int	Server::joinChan(User *user, std::vector<std::string> args)
{
//	If join have no channel name, it return "#". We use "#" to return an error code.
	if (args.size() < 2 || args[1].compare("#") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else
	{
		std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	//	if the channel exists, try to join it. else create it
		if (it != this->_chanContainer.end())
			knownChannel(user, it->second, args);
		else
			newChannel(user, args);
	}
	return (0);
}


int	Server::leaveChan(User *user, std::vector<std::string> args)
{
//	If join have no channel name, it return "#". We use "#" to return an error code.
	if (args.size() < 2 || args[1].compare("#") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else
	{
		std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	//	if the channel exists, try to join it. else create it
		if (it != this->_chanContainer.end())
			removeFromChan(user, NULL, it->second);
		else
			sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "Channel does not exist"));
	}
	return (0);
}


//	When client quits the server
int	Server::quitServer(User *user, std::vector<std::string> args)
{
	(void)args;
	this->deleteClient(user->getFD());
	return (0);
}


int	Server::inviteUser(User *user, std::vector<std::string> args)
{
	User *invitee = findUser(args[1]);
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[2]);

	if (args.size() < 3)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "Channel does not exist"));
	else if (invitee == NULL)
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHNICK, "Invitee does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "User is not in channel (cannot invite others)"));
	else if (it->second->hasMember(invitee))
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTERED, "Invitee is already in channel"));
	else if (it->second->getInviteFlag() == 1 && !(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, ERR_NOPRIVILEGES, "Invite only channel : operator permissions needed"));
	else if (checkMaxMbr(user, it->second))
	{
		sendToUser(invitee, makeUserMsg(user, "INVITE", args[2]));
		dragToChannel(invitee, it->second);
	}
	return (0);
}

int	Server::kickUser(User *user, std::vector<std::string> args)
{
//	Finding user that would be kicked in chan
	User *member = findUser(args[2]);
//	Finding channel

	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	if (args.size() < 3)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "Channel does not exist"));
	else if (member == NULL)
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHNICK, "Member does not exist"));
	else if (!(it->second->hasMember(member)))
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "User is not in channel"));
	else if (!(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));//prob
	else
		removeFromChan(member, user, it->second);
	return (0);
}



int	Server::setChanTopic(User *user, std::vector<std::string> args)
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	if (args.size() < 2 || args[1].compare("#") == 0 || args[1].compare(":") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "No such channel"));
	//		Tells people what the topic is
	else if (args.size() == 2 && it != this->_chanContainer.end())
	{
		std::string input = user->getUsername() + " " + it->second->getChanName() + " :" + it->second->getTopic();
		sendToUser(user, makeChanMsg(user, RPL_TOPIC, input));
	}
	else if (it->second->getTopicFlag() == 1 && !(it->second->isChanOp(user)))
		sendToUser(user, makeUserMsg(user, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
	//		Sets the topic
	else if (it->second->getTopicFlag() == 0 || it->second->isChanOp(user))
	{
		it->second->setTopic(args[2]);
		it->second->tellChanTopic(user);
	}
	return (0);
}

int	Server::setChanMode(User *user, std::vector<std::string> args) //	TODO : used tellChanMode() for other mods than o
{
//	Finding channel
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

//	If it found channel and user is chanop
	if (args.size() == 2) //	to avoid limechat fuckery
		return (0);
	if (args.size() <= 1)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "Channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "User is not in channel"));
	else if (!it->second->isChanOp(user))
		sendToUser(user, makeUserMsg(user, ERR_CHANOPRIVSNEEDED, "Operator permissions needed"));
	//		Checks and executes the mode cmd
	else
	{
//		if modestring is NOT size 2 OR not + OR - => invalid mode
		if (args[2].size() != 2 || (args[2][0] != '+' && args[2][0] != '-'))
		{
			sendToUser(user, makeUserMsg(user, ERR_UMODEUNKNOWNFLAG, "Invalid mode flag"));
			return (0);
		}
//		mode i (chanop can invite or not)
		else if (args[2][1] == 'i')
		{
			if (args[2][0] == '+')			it->second->setInviteFlag(1);
			else if (args[2][0] == '-')		it->second->setInviteFlag(0);
			it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2]), true);
		}
//		mode t (topic can only be change by chanop or not)
		else if (args[2][1] == 't')
		{
			if (args[2][0] == '+')			it->second->setTopicFlag(1);
			else if (args[2][0] == '-')		it->second->setTopicFlag(0);
			it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2]), true);
		}
//		mode k (password is set by chanop on channel or not having password)
		else if (args[2][1] == 'k')
		{
			if (args.size() <= 4)
			{
				sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
				return (0);
			}
			if (args[2][0] == '+')
			{
				it->second->setKeyFlag(1);
				it->second->setPass(args[3]);
			}
			else if (args[2][0] == '-')
			{
				it->second->setKeyFlag(0);
				it->second->setPass("");
			}
			it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2]), true);
		}
//		mode o (add or remove chanOp privileges)
		else if (args[2][1] == 'o')
		{
			if (args.size() <= 4)
			{
				sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
				return (0);
			}
			User *invitee = findUser(args[3]);

			if (!it->second->hasMember(invitee))
			{
				sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Member not in channel"));
				return (0);
			}
			else if (args[2][0] == '+')	it->second->addChanOp(invitee);
			else if (args[2][0] == '-')	it->second->removeChanOp(invitee);
		}
//		mode l (set member limit or remove member limit by chanop on channel)
		else if (args[2][1] == 'l')
		{
			if (args[2][0] == '-')
			{
				it->second->setMaxMemberCount(0);
				it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2]), true);
				return (0);
			}
			if (args.size() <= 4)
			{
				sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
				return (0);
			}
			if (args[3].find_first_not_of("0123456789") != std::string::npos)
			{
				sendToUser(user, makeUserMsg(user, ERR_UNKNOWNERROR, "Needs to be a number"));
				return (0);
			}
			int count = stoi(args[3]);
			it->second->setMaxMemberCount(count);

			it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2] + " " + args[3]), true);

		}
		else
		{
			sendToUser(user, makeUserMsg(user, ERR_UNKNOWNMODE, "Invalid mode character"));
			return (0);
		}
//		Informs the channel that the mode change happened
//		if (args[2][1] != 'o') //	avoids sending the message twice, since add/removeChanOp() does it itself
//			it->second->sendToChan(user, makeChanMsg(user, "MODE " + it->second->getChanName(), args[2]), true);
	}
	return (0);
}

//	PRIVMSG #channel/user <message>
int	Server::sendMessage(User *user, std::vector<std::string> args)
{
	if (args.size() < 3)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (args[1][0] == '#')
		processChanMsg(user, args);
	else
		processPrivMsg(user, args);
	return (0);
}

//	TELLS THE SERVER TO SHUT OFF (FOR CORRECTION PPURPOSES)
int	Server::closeServer(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;
	this->shutOff();
	return (0);
}

int	Server::pong(User *user, std::vector<std::string> args)
{
	(void)args;
	(void)user;
	return (0);
}

//	GETS THE SPECIFIC ID OF A USER COMMAND
int Server::getCmdID(User *user, std::string cmd)
{
	std::string cmds[CMD_COUNT] = { "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "QUIT", "INVITE", "TOPIC", "MODE", "PRIVMSG", "CLOSESERV", "PONG"};

	int id = 0;
	while (id < CMD_COUNT && cmd.compare(cmds[id]))
		id++;

	// if user is using a valid command without being logged in, we tell them to log in first
	if (id > 2 && id != CMD_COUNT && !user->isLoggedIn())
		return (CMD_COUNT + 1);

	return (id);
}

//	PICKS A COMMAND TO EXECUTE BASED ON THE ARGS
int	Server::execCommand(User *user, std::vector<std::string> args)
{
	debugPrint(MAGENTA, "EXECUTING MSG :\n" + user->getLastMsg()); // 		DEBUG

	int (Server::*commands[])(User*, std::vector<std::string>) = {
		&Server::checkPassword,
		&Server::storeNickname,
		&Server::storeUserInfo,
		&Server::joinChan,
		&Server::leaveChan,
		&Server::kickUser,
		&Server::quitServer,
		&Server::inviteUser,
		&Server::setChanTopic,
		&Server::setChanMode,
		&Server::sendMessage,
		&Server::closeServer,
		&Server::pong, //				NOTE : to prevent limechat from filling up the logs >:(
		&Server::notACommand, //		NOTE : default case for getCmdID() (returns -1)
		&Server::notLoggedIn //			NOTE : command N : for loggin perms
	};
	return (this->*commands[getCmdID(user, args[0])])(user, args);
}

//	MAKES execCommand RETURN AN ERROR CODE (1): invalid command
int	Server::notACommand(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;
	return (1);
}

//	PREVENTS USER FOR ACCESSING NON_LOGGIN CMDS WHEN NOT LOGGED IN
int	Server::notLoggedIn(User *user, std::vector<std::string> args)
{
	(void)args;

	sendToUser(user, makeUserMsg(user, ERR_NOTREGISTERED, "Forbidden command : finish logging in first"));

	return (0);
}
