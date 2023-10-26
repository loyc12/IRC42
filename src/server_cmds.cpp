#include "IRC.hpp"

// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, std::vector<std::string> args)
{
	if (user->wasWelcomed)
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTRED, "Already registered"));
	else if (args[1].compare(std::to_string(this->getPass())) != 0)
	{
		std::cerr << "Pass  : " << std::to_string(this->getPass()) << "." << std::endl << "Input : " << args[1] << "." << std::endl; //			DEBUG

		sendToUser(user, makeUserMsg(user, ERR_PASSWDMISMATCH, "Invalid password"));
		deleteClient(user->getFD());
	}
	return (0);
}

int	Server::storeNickname(User *user, std::vector<std::string> args)
{
	if (isNickValid(user, args[1])) //	NOTE : this send its own error messages
	{
		std::string tmp = user->getNick();
		user->setNick(args[1]);
	}
	else if (!user->wasWelcomed)
		deleteClient(user->getFD());
	return (0);
}

int	Server::storeUserInfo(User *user, std::vector<std::string> args)
{
	user->setUserInfo(args);

//	Welcomes as user if this is their first password check (aka first connection)
	if (!user->wasWelcomed)
		this->welcomeUser(user);
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
		{
			(it->second)->sendToChan(user, makeChanMsg(user, "PART", (it->second)->getChanName()), true);	//	1st : tell channel they left
			(it->second)->removeMember(user); //																2nd : remove user from channel
			(it->second)->updateMemberList(user); //															3rd : update member list for all members
		}
		else
			sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "channel does not exist"));
	}
	return (0);
}


int	Server::kickUser(User *user, std::vector<std::string> args)
{
//	Finding user that would be kicked in chan
	User *member = findUser(args[2]);
//	Finding channel
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	if (member == NULL)
		sendToUser(user, makeUserMsg(user, "ERR_NOSUCHNICK", "member does not exist"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, "ERR_CANNOTSENDTOCHAN", "user is not in channel (cannot invite others)"));
	else if (!(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, "ERR_CHANOPRIVSNEEDED", "not a chan op"));
	else
	{
		std::cout << "> KICKING " << member->getNick() << " out of " << args[1] << std::endl; //					DEBUG
		it->second->sendToChan(member, makeChanMsg(user, "KICK", args[1] + " " + args[2] + " :" + user->getNick()), true);
		it->second->removeMember(member);
		it->second->updateMemberList(member);
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

	if (invitee == NULL)
		sendToUser(user, makeUserMsg(user, "ERR_NOSUCHNICK", "invitee does not exist"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, "ERR_CANNOTSENDTOCHAN", "user is not in channel (cannot invite others)"));
	else if (it->second->hasMember(invitee))
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTERED, "invitee is already in channel"));
	else if (!(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, "ERR_NOPRIVILEGES", "not a chan op"));
	else if (it->second->getInviteFlag() == 1 && (!(it->second->hasChanOp(user))))
		sendToUser(user, makeUserMsg(user, "ERR_CHANOPRIVSNEEDED", "invite only channel"));
	else if (it->second->hasChanOp(user) && checkMaxMbr(user, it->second))
	{
		sendToUser(invitee, makeUserMsg(user, "INVITE", args[2]));
		dragToChannel(invitee, it->second);
	}
	return (0);
}


int	Server::setChanTopic(User *user, std::vector<std::string> args)
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	if (args.size() < 2 || args[1].compare("#") == 0 || args[1].compare(":") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "No such channel"));
	else if (!(it->second->isChanOp(user)) && args.size() == 3 && it->second->getTopicFlag() == 1)
		sendToUser(user, makeUserMsg(user, "ERR_CHANOPRIVSNEEDED", "not a chan op"));
	else if (args.size() == 2 && it != this->_chanContainer.end()) //	NOTE: for anyone who wants to know the topic of chan CMD sent: TOPIC #chanName
	{
		std::string input = it->second->getChanName() + " :" + it->second->getTopic();
		sendToUser(user, makeUserMsg(user, "TOPIC", input));
	}
	else if ((it->second->getTopicFlag() == 1 && it->second->isChanOp(user)) || (it->second->getTopicFlag() == 0))
	{
		it->second->setTopic(args[2]);
		std::string input = it->second->getChanName() + " :" + it->second->getTopic();
		it->second->sendToChan(user, makeChanMsg(user, "TOPIC", input), true);
	}
	return (0);
}

int	Server::setChanMode(User *user, std::vector<std::string> args)
{
//	Finding channel
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

//	If it found channel and user is chanop
	if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "channel does not exist"));
	else if (it->second->isChanOp(user))
	{
		if (args.size() == 2)
			return (0);
//		if modestring is NOT size 2 OR not + OR - => invalid mode
		else if (args[2].size() != 2 || (args[2][0] != '+' && args[2][0] != '-'))
		{
			sendToUser(user, makeUserMsg(user, "ERR_UMODEUNKNOWNFLAG", "invalid mode"));
			return (0);
		}
//		mode i (chanop can invite or not)
		else if (args[2][1] == 'i')
		{
			if (args[2][0] == '+')			it->second->setInviteFlag(1);
			else if (args[2][0] == '-')		it->second->setInviteFlag(0);
		}
//		mode t (topic can only be change by chanop or not)
		else if (args[2][1] == 't')
		{
			if (args[2][0] == '+')			it->second->setTopicFlag(1);
			else if (args[2][0] == '-')		it->second->setTopicFlag(0);
		}
//		mode k (password is set by chanop on channel or not having password)
		else if (args[2][1] == 'k' && (args.size() == 4))
		{
			if (args[2][0] == '+')
				it->second->setPass(args[3]);
			else if (args[2][0] == '-')
				it->second->setPass("");
		}
//		mode o (add or remove chanOp privileges)
		else if (args[2][1] == 'o' && (args.size() == 4))
		{
			User *invitee = findUser(args[3]);
			if (it->second->hasMember(invitee))
			{
				if (args[2][0] == '+')
				{
					it->second->addChanOp(invitee);
					std::string chanOp = "+o " + invitee->getNick();
					sendToUser(invitee, makeUserMsg(invitee, "MODE", chanOp));
				}
				else if (args[2][0] == '-')
				{
					it->second->removeChanOp(invitee);
					std::string chanOp = "-o " + invitee->getNick();
					sendToUser(invitee, makeUserMsg(invitee, "MODE", chanOp));
				}
			}
			else
				sendToUser(user, makeUserMsg(user, "ERR_CANNOTSENDTOCHAN", "Member not in channel"));
		}
//		mode l (set member limit or remove member limit by chanop on channel)
		else if (args[2][1] == 'l')
		{
			if (args[2][0] == '+')
			{
				if (args.size() > 3 && (args[3].find_first_not_of("0123456789") == std::string::npos))
				{
					int count = stoi(args[3]);
					it->second->setMaxMemberCount(count);
				}
				else
				{
					sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));
					return (0);
				}
			}
			else if (args[2][0] == '-')
				it->second->setMaxMemberCount(0);
		}
		else
		{
			sendToUser(user, makeUserMsg(user, "ERR_UNKNOWNCOMMAND", "invalid mode"));
			return (0);
		}
//		Informs the user that the mode change was successful
		std::string command = args[1] + " " + args[2] + " " + user->getNick();
		sendToUser(user, makeUserMsg(user, "MODE", command));
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
	this->shutoff = true;
	debugPrint(MAGENTA, "\n\n > Force-closing...\n"); //			DEBUG
	//this->clear();

	debugPrint(MAGENTA, "\n\n > Closing (manually) and cleaning ...\n"); //						DEBUG
	this->clear();
	return (0);
}


//	MAKES execCommand RETURN AN ERROR CODE (1)
int	Server::notACommand(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;
	return (1); // aka : invalid command
}

//	GETS THE SPECIFIC ID OF A USER COMMAND
int Server::getCmdID(std::string cmd)
{
	std::string cmds[CMD_COUNT] = { "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "QUIT", "INVITE", "TOPIC", "MODE", "PRIVMSG", "CLOSESERV"};

	int id = 0;
	while (id < CMD_COUNT && cmd.compare(cmds[id]))
		id++;

	return (id);
}


//	PICKS A COMMAND TO EXECUTE BASED ON THE ARGS
int	Server::execCommand(User *user, std::vector<std::string> args)
{
	debugPrint(MAGENTA, "EXECUTING MSG :\n" + user->lastMsg); // 			DEBUG

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
		&Server::notACommand //											NOTE : default case for getCmdID()
	};
	return (this->*commands[getCmdID(args[0])])(user, args);
}
