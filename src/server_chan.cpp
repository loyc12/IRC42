#include "IRC.hpp"



void	Server::knownChannel(User *user, Channel *chan, std::vector<std::string> args)
{
//	Check all conditions in mode if we can add the member to this channel
	if (!isUserInChan(user, chan) && checkInvitePerm(user, chan) \
		&& checkPass(user, chan, args[2]) && checkMaxMbr(user, chan)) //	NOTE : these send their own error messages
	{
//		the client can enter the channel
		debugPrint(MAGENTA, "\n > joinning a channel\n"); // DEBUG
		chan->addMember(user);
		chan->replyToChan(user, JOIN, chan->getChanName());
	}
}



void	Server::newChannel(User *user, std::vector<std::string> args)
{
 		if (args.size() > 2)
 		{
//			If more args are added, we assume the client wanted to join a channel, so we block the creation
 			replyTo(REQUEST, user, user, "403", ERR_NOSUCHCHANNEL);
 		}
		else //	create the channel
		{
			//TODO create function in channel to do that
			debugPrint(MAGENTA, "\n > adding a new channel\n"); //										DEBUG
			Channel *newChannel = new Channel(args[1]); //												WARNING : may need to deal with leaks
			this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));
			newChannel->setChanName(args[1]);
			newChannel->addMember(user);
			replyTo(CHAN, user, user, JOIN, newChannel->getChanName());
	//		Re-using setUsermode for automation
			std::cout << "newChannel: invite or not? " << newChannel->getInviteFlag() << std::endl;//	DEBUG
			newChannel->setAdminName(user->getNick());
			newChannel->addChanOps(user);
			std::string chanOp = "o " + user->getNick();
			std::cout << "chanOP string: " << chanOp << std::endl;
			replyTo(REQUEST, user, user, "MODE", chanOp);
		}
		//setUserMode(user, args, newChannel, "NEW");
}



//	SENDS A SINGLE MESSAGE TO ALL MEMBERS OF A CHANNEL
void	Server::sendToChan(User *fromUser, std::string message, std::vector<std::string> args)
{
	Channel *chan = findChannel(args[1]);

//	Sends a message to every channel member if it has at least 3 args (PRIVMSG + chan + message[0])
	if (chan != NULL && args.size() > 2)
	{
		std::cerr << "sendToChan(), chan not NULL : " << args[1] << std::endl; //			DEBUG
		for (int i = 0; i < chan->getMemberCnt(); i++)
		{
			User *toUser = chan->getMember(i);

			//	Prevents sending the message to the sender or bad FD
			if (toUser != NULL && toUser != fromUser)
			{
				std::cerr << "sending chan msg to : " << toUser->getNick() << " ("; //		DEBUG
				std::cerr << i << ")" << " : " << message << std::endl; //					DEBUG
				replyTo(CHAN, fromUser, toUser, "", message);
			}
		}
	}
}
