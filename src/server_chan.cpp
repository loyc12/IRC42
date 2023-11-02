#include "IRC.hpp"


void	Server::knownChannel(User *user, Channel *chan, std::vector<std::string> args)
{
//	Check all conditions in mode if we can add the member to this channel
	if (chan->getKeyFlag() == 1)
	{
		if (!isUserInChan(user, chan) && checkInvitePerm(user, chan) \
		&& checkPass(user, chan, args[2]) && checkMaxMbr(user, chan))
			addToChan(user, chan);
	}
	else if (chan->getKeyFlag() == 0)
	{
		if (!isUserInChan(user, chan) && checkInvitePerm(user, chan) && checkMaxMbr(user, chan))
			addToChan(user, chan);
	}
}

void	Server::newChannel(User *user, std::vector<std::string> args)
{
//	If more args are added, we assume the client wanted to join a channel, so we block the creation
	if (args.size() > 2)
		sendToUser(user, makeUserMsg(user, ERR_NOSUCHCHANNEL, "No such channel"));
	else //	create the channel
	{
		Channel *newChannel = new Channel(args[1]);
		this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));
		newChannel->setChanName(args[1]);

		addToChan(user, newChannel);

		newChannel->setAdminName(user->getNick());
		newChannel->addChanOp(user, user);
	}
}

void	Server::dragToChannel(User *invitee, Channel *chan)
{
//	Check all conditions in mode if we can add the member to this channel
	if (!isUserInChan(invitee, chan) && checkMaxMbr(invitee, chan)) //	NOTE : these send their own error messages
	{
//		the client can enter the channel
		debugPrint(MAGENTA, "\n > inviting (dragging) " + invitee->getNick() + " to chan " + chan->getChanName() + "\n"); // DEBUG

		addToChan(invitee, chan);
	}
}

void	Server::processChanMsg(User *sender, std::vector<std::string> args)
{
	Channel *chan = findChannel(args[1]);

//	Sends a message to every channel member if it has at least 3 args (PRIVMSG + chan + message[0])
	if (chan == NULL)
		sendToUser(sender, makeUserMsg(sender, ERR_NOSUCHCHANNEL, "Channel does not exist"));
	else
	{
		if (!chan->hasMember(sender))
			sendToUser(sender, makeUserMsg(sender, ERR_CANNOTSENDTOCHAN, "You are not in this channel"));
		else
			chan->sendToChan(sender, makeChanMsg(sender, sender->getLastMsg()), false);
	}
}

void	Server::processPrivMsg(User *sender, std::vector<std::string> args)
{
	User *receiver = findUser(args[1]);

	if (receiver == NULL)
		sendToUser(sender, makeUserMsg(sender, ERR_ERRONEUSNICKNAME, "Nickname does not exist"));
	else
		sendToUser(receiver, makePrivMsg(sender, sender->getLastMsg()));
}

//		THESE 3 COULD BE IN CHANNEL.CPP

void	Server::addToChan(User *user, Channel *chan)
{
	chan->addMember(user); //															1st : add user to channel
	chan->sendToChan(user, makeChanMsg(user, "JOIN", chan->getChanName()), true);	//	2nd : tell channel they joined
	chan->updateMemberList(user, false); //												3rd : update member list for all members
}

void	Server::removeFromChan(User *member, Channel *chan) { this->removeFromChan(member, NULL, chan); }
void	Server::removeFromChan(User *member, User *kicker, Channel *chan)
{
	chan->updateMemberList(member, true); //											2nd : update member list for all members

	if (kicker)	//																		1st : tell channel they left
		chan->sendToChan(member, makeChanMsg(kicker, "KICK", chan->getChanName() + " " + member->getNick() + " :" + kicker->getNick()), true);
	else
		chan->sendToChan(member, makeChanMsg(member, "PART", chan->getChanName()), true);

	chan->removeMember(member); //														3rd : remove user from channel

}
