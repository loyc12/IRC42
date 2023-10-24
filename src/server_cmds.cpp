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
			newChannel(user, args);//	WARNING : WHAT DOES IT DO IF WE ADD A PASSWORD HERE ?
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
//	Finding user that would be kicked in chan
	User *member = findUser(args[2]);
//	Finding channel
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	std::cout << "> KICKING " << member->getNick() << " out of " << args[1] << std::endl; //					DEBUG

	if (member == NULL)
		sendToUser(user, makeUserMsg(user, "401", "member does not exist"));
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	else if (!(it->second->hasMember(user)))
		sendToUser(user, makeUserMsg(user, "404", "user is not in channel (cannot invite others)"));
	else if (!(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, "482", "not a chan op"));
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
	else if (!(it->second->hasChanOp(user)))
		sendToUser(user, makeUserMsg(user, "482", "not a chan op"));
	else if (it->second->getInviteFlag() == 1 && (!(it->second->hasChanOp(user))))
		sendToUser(user, makeUserMsg(user, "482", "invite only channel"));
	else if (it->second->hasChanOp(user))
	{
		sendToUser(invitee, makeUserMsg(user, "INVITE", args[2]));
		dragToChannel(invitee, it->second);
	}
	return (0);
}


//TOPIC #channel <new topic>
int	Server::setChanTopic(User *user, std::vector<std::string> args)//	WARNING: when chan op changes TOPIC, it appears twice, not in the chan... in log window
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	
	if (args.size() < 2 || args[1].compare("#") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more parameters"));	
	else if (it == this->_chanContainer.end())
		sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	else if (!(it->second->isChanOp(user)) && args.size() == 3 && it->second->getTopicFlag() == 1)
		sendToUser(user, makeUserMsg(user, "482", "not a chan op"));
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
//	(void)user;
// 	MODE <#channel_name> <+/- code_en_question> <password/autre>
	
//	Finding channel
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	std::cout << "chanName: " << it->second->getChanName() << std::endl; //				DEBUG
	
	if (it != this->_chanContainer.end() && it->second->isChanOp(user))
	{
		// if (!(it->second->hasChanOp(user))) //	Might need to change
		// {
		// 	sendToUser(user, makeUserMsg(user, "482", "not a chan op"));
		// 	return (0);
		// }
		if (args.size() == 2)
		{
// 			RPL_CHANNELMODEIS (324) 
//   		"<client> <channel> <modestring> <mode arguments>..."
// 			Sent to a client to inform them of the currently-set modes of a channel.
//			<channel> is the name of the channel. 
//			<modestring> and <mode arguments> are a mode string and the mode arguments(delimited as separate parameters) as defined in the MODE message description.


			//set mode
			//check tous les flags sont à quoi
			std::cout << "client veut juste savoir le mode" << std::endl;
			//sendToUser(user, makeUserMsg(user, "324", "329")); 				//TODO need to come back to me later
			return (0);
		}
		else if (args[2].size() != 2 || (args[2][0] != '+' && args[2][0] != '-')) //1. modestring is NOT size 2 OR not + OR - => invalid mode
		{
			std::cout << "invalid mode" << std::endl; //						DEBUG
			sendToUser(user, makeUserMsg(user, "501", "invalid mode"));//ERR_UMODEUNKNOWNFLAG 
			return (0);
		}
		else if (args[2][1] == 'i')
		{
			//std::cout << "in i" << std::endl;
			if (args[2][0] == '+')			it->second->setInviteFlag(1);
			else if (args[2][0] == '-')		it->second->setInviteFlag(0);
		}
		else if (args[2][1] == 't')
		{
			if (args[2][0] == '+')			it->second->setTopicFlag(1); //				NOTE only chanOp can change TOPIC
			else if (args[2][0] == '-')		it->second->setTopicFlag(0); //				NOTE anyone in the channel can change the TOPIC
		}
		else if (args[2][1] == 'k' && (args.size() == 4))
		{
			if (it == this->_chanContainer.end())
				sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
			else if (args[2][0] == '+')
				it->second->setPass(args[3]);
			else if (args[2][0] == '-')
				it->second->setPass(""); //					NOTE no password
		}
		else if (args[2][1] == 'o' && (args.size() == 4)) //	NOTE: review print msg or send msg to User and error msg										
		{
			User *invitee = findUser(args[3]);

			if (it == this->_chanContainer.end())
				sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
			else
			{
				if (it->second->hasMember(invitee)) //			chanOP give the chanOP privileges to someone else
				{
					if (args[2][0] == '+')
							it->second->addChanOp(invitee);
					else if (args[2][0] == '-')
							it->second->removeChanOp(invitee);
				}
				else
					sendToUser(user, makeUserMsg(user, "404", "Member not in channel"));
			}
		}
		else if (args[2][1] == 'l') //											WARNING: when implemented, need to add checkups in cmd JOIN chan
		{
//			if (args[2][0] == '+')	it->second->setInviteFlag(1); //			NOTE set a limit to the channel
//			else					it->second->setInviteFlag(0); //			NOTE no more limit
		}
		else // peut-être pas besoin de ce else généra
		{
			std::cout << "invalid mode" << std::endl; //						TODO : send me to user as an error
			//sendToUser(user, makeUserMsg(user, "MODE", "invalid mode"));
			return (0);
		}
		//std::cout << "main if, at the end" << std::endl; //									DEBUG
//		Informs the user that the mode change was successful
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
