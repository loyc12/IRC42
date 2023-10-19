#include "IRC.hpp"



// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, std::vector<std::string> args)
{
	if (user->wasWelcomed)
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTRED, "Already registered"));
	else if (args[1].compare(this->getPass()) != 0)
	{
		sendToUser(user, makeUserMsg(user, ERR_PASSWDMISMATCH, "Invalid password"));
		deleteClient(user->getFD());
	}
	return (0);
}



int	Server::storeNickname(User *user, std::vector<std::string> args)
{
	if (isNickValid(user, args[1])) //	NOTE : this send its own error messages
		user->setNick(args[1]);
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
	std::cout << "args[1]: " << args[1] << " " << args[1].length() << std::endl; //								DEBUG
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
			sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	}
	return (0);
}


int	Server::kickUser(User *user, std::vector<std::string> args)
{
	User *member = findUser(args[2]);
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	std::cout << "> KICKING " << member->getNick() << " out of " << args[1] << std::endl; //					DEBUG

	if (member == NULL)
		sendToUser(user, makeUserMsg(user, "401", "member does not exist"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, "404", "user is not in channel (cannot invite others)"));
	else
	{
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
	(void)user;

	User *invitee = findUser(args[1]);
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[2]);

	if (invitee == NULL)
		sendToUser(user, makeUserMsg(user, "401", "invitee does not exist"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, "404", "user is not in channel (cannot invite others)"));
	else if (it->second->hasMember(invitee))
		sendToUser(user, makeUserMsg(user, "462", "invitee is already in channel"));
	else
	{
		sendToUser(invitee, makeUserMsg(user, "INVITE", args[2]));
		dragToChannel(invitee, it->second);
	}
	return (0);
}



int	Server::setChanTopic(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : set channel topic" << std::endl; //								DEBUG

	return (0);
}



int	Server::setChanMode(User *user, std::vector<std::string> args)
{
	(void)user;
	// MODE #channel_name +/- code_en_question

	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	if (it != this->_chanContainer.end())
	{
		if (args[2].size() != 2 || (args[2][0] != '+' && args[2][0] != '-'))
		{
			std::cout << "invalid mode" << std::endl; //						TODO : send me to user as an error
			return (0);
		}
		else if (args[2][1] == 'i')
		{
			if (args[2][0] == '+')	it->second->setInviteFlag(1);
			else					it->second->setInviteFlag(0);
		}
		else if (args[2][1] == 't')
		{
//			if (args[2][0] == '+')	it->second->setInviteFlag(1);
//			else					it->second->setInviteFlag(0);
		}
		else if (args[2][1] == 'k')
		{
//			if (args[2][0] == '+')	it->second->setInviteFlag(1);
//			else					it->second->setInviteFlag(0);
		}
		else if (args[2][1] == 'o')
		{
//			if (args[2][0] == '+')	it->second->setInviteFlag(1);
//			else					it->second->setInviteFlag(0);
		}
		else if (args[2][1] == 'l')
		{
//			if (args[2][0] == '+')	it->second->setInviteFlag(1);
//			else					it->second->setInviteFlag(0);
		}
		else
		{
			std::cout << "invalid mode" << std::endl; //						TODO : send me to user as an error
			return (0);
		}

//		Informs the user that the mode cange was successful
		std::string command = args[1] + " " + args[2] + " " + user->getNick();
		sendToUser(user, makeUserMsg(user, "MODE", command));

		std::cout << "Set "  << args[2][1] << " mode to " << args[2][0] << std::endl; //	DEBUG
	}
	else
		std::cout << "invalid channel" << std::endl; //							TODO : send me to user as an error

	return (0);
}



//	TELLS readFromClient() THAT THIS IS NOT A COMMAND (aka it's a message)
int	Server::notACommand(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;
	return (-1);
}



//	GETS THE SPECIFIC ID OF A USER COMMAND
int Server::getCmdID(std::string cmd)
{
	std::string cmds[CMD_COUNT] = { "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "QUIT", "INVITE", "TOPIC", "MODE" };

	int id = 0;
	while (id < CMD_COUNT && cmd.compare(cmds[id]))
		id++;

	return (id);
}


//	PICKS A COMMAND TO EXECUTE BASED ON THE ARGS
int	Server::execCommand(User *user, std::vector<std::string> args)
{
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
		&Server::notACommand //											NOTE : default case for getCmdID()
	};
	return (this->*commands[getCmdID(args[0])])(user, args);
}
