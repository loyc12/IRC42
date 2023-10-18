#include "IRC.hpp"



// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, std::vector<std::string> args)
{
	if (user->wasWelcomed)
		sendToUser(user, makeUserMsg(user, ERR_ALREADYREGISTRED, "Already registered"));
	else if (args[1].compare(this->getPass()) != 0)
	{
//		If password is invalid
		std::string errMsg = "Invalid password";
		user->setNick("");
		sendToUser(user, makeUserMsg(user, ERR_PASSWDMISMATCH, errMsg));

//		Deletes the client, loses its FD and removes it from the baseFds
		deleteClient(user->getFD());
	}
	return (0);
}



int	Server::storeNickname(User *user, std::vector<std::string> args)
{
	user->setNick(args[1]);
	//TODO : si la fonction nick a deja trouver un nickname pareil, fuck you
	//?? sauf que cette fonction est appelé au début lorsqu'il se connecte au serveur. Je crois qu'à ce point là, 2 nicknames pareil peuvent exister... À moins qu'on dit qu'on le gère de même, point final??
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
	std::cout << "args[1]: " << args[1] << " " << args[1].length() << std::endl; //						DEBUG
	if (args.size() < 2 || args[1].compare("#") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more params"));
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
	std::cout << "args[1]: " << args[1] << " " << args[1].length() << std::endl; //						DEBUG
	if (args.size() < 2 || args[1].compare("#") == 0)
		sendToUser(user, makeUserMsg(user, ERR_NEEDMOREPARAMS, "Need more params"));
	else
	{
		std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	//	if the channel exists, try to join it. else create it
		if (it != this->_chanContainer.end())
		{
			(it->second)->sendToChan(user, makeChanMsg(user, "PART", (it->second)->getChanName()), true);	//	1nd : tell channel they left
			(it->second)->updateMemberList(user); //															2rd : update member list for all members
			(it->second)->removeMember(user); //																3st : remove user from channel
		}
		else
			sendToUser(user, makeUserMsg(user, "403", "channel does not exist"));
	}
	return (0);
}



int	Server::kickUser(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : kick user out" << std::endl; //				DEBUG

	return (0);
}



//	When client quits the server
int	Server::quitServer(User *user, std::vector<std::string> args)
{
	(void)args;
	this->deleteClient(user->getFD());
	//											TODO : remove me from all channels too
	return (0);
}



int	Server::inviteUser(User *user, std::vector<std::string> args)
{
	(void)user;
	// INVITE USER channel

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
		sendToUser(invitee, makeUserMsg(user, "341", args[2]));
		dragToChannel(invitee, it->second);
	}
	return (0);
}



int	Server::setChanTopic(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : set channel topic" << std::endl; //						DEBUG

	return (0);
}



int	Server::setChanMode(User *user, std::vector<std::string> args)
{
	(void)user;
	// MODE #channel_name +/- code_en_question
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	if (it != this->_chanContainer.end())
	{
		/*
		TODO ?
		if i
			chan->setTo(c, arg(?));
		if t
			chan->setTo(c, arg(?));
		if k
			chan->setTo(c, arg(?));
		if o
			chan->setTo(c, arg(?));
		if l
			chan->setTo(c, arg(?));
		*/
		if (args[2].compare("+i") == 0 || args[2].compare("-i") == 0)
		{
			if (args[2][0] == '+')
				it->second->setInviteFlag(1);
			else
				it->second->setInviteFlag(0);

			std::cout << "Flag set" << std::endl; //								DEBUG

			std::string command = args[1] + " " + args[2] + " " + user->getNick();
			sendToUser(user, makeUserMsg(user, "MODE", command));
		}
	}
	return (0);
}



//	TELLS readFromClient() THAT THIS IS A MESSAGE
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
		&Server::notACommand //												NOTE : default case for getCmdID()
	};
	debugPrint(RED, args[0]); // 											DEBUG
	return (this->*commands[getCmdID(args[0])])(user, args);
}