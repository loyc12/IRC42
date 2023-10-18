#include "IRC.hpp"

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user) //									TODO (LL) : look over
{
	sendToUser(user, makeUserMsg(user, RPL_WELCOME, WELCOME_HEADER));
	user->wasWelcomed = true;
}



void	Server::readFromClient(User *user, int fd, std::string *lastMsg)
{
	char 		buff[BUFFSIZE];

	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Handles what to do depending on the byte value (error, null or message)
	if (byteReceived == -1)
	{
//		If CTRL-C at recv, treat as not an error; in Netcat
		if (errno == EINTR)
			throw std::invalid_argument(" > Error at select(): ");
	}
	else if (byteReceived == 0)
	{
//		Deletes the client, loses its FD and removes it from the baseFds
		deleteClient(fd);
	}
	else if (byteReceived > 0)
	{
        lastMsg->assign(buff, 0, byteReceived);

		std::vector<std::string> args = splitString(buff, " \r\n");
		if (execCommand(user, args) == -1)
		{
        	std::ostringstream debug; //											DEBUG
        	debug << "INCOMING MSG FROM : (" << fd << ")\t| " << *lastMsg; //		DEBUG
        	debugPrint(GREEN, debug.str()); //										DEBUG

			//	if is a channel message : send to channel users
			if (args[0].compare("PRIVMSG") == 0)
				processChanMsg(user, *lastMsg, args);
			else
        		sendToUser(user, makeUserMsg(user, *lastMsg));
		}
	}
	bzero(buff, BUFFSIZE);
}


/*
//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	Server::replyTo(int target, User* fromUser, User* toUser, std::string code, std::string input) //		TODO (LL) : overload this so its not one spaghetti fct
{
	std::ostringstream 	message;
	std::string 		result;

//	send structured fix template message to infobox of client (request) or to a chan of client (CHAN) (DONT TOUCH)
	if (target == REQUEST)
		message << ":" << fromUser->getHostname() << " " << code << " " << fromUser->getNick() << " :" << input << "\r\n";
	else if (target == CHAN)
		message << ":" << fromUser->getNick() << "!" << fromUser->getUsername() << "@" << fromUser->getHostname() << " " << code << " " << input << "\r\n"; // FOR WELCOME ONLY ??

	result = message.str();
	//std::ostringstream debug; //														DEBUG
	//debug << "OUTGOING C_MSG TO : (" << toUser->getFD() << ")\t| " << result; //		DEBUG
	debugPrint(GREEN, result); //														DEBUG

	if (send(toUser->getFD(), result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at replyTo() ");
}
*/


//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	Server::sendToUser(User* targetUser, std::string message)
{
	std::ostringstream debug; //															DEBUG
	debug << "OUTGOING USER_MSG TO : (" << targetUser->getFD() << ")\t| " << message; //	DEBUG
	debugPrint(GREEN, debug.str()); //															DEBUG

	if (send(targetUser->getFD(), message.c_str(), message.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToUser() ");
}


/*
//	NOTE : need to overload for : (reason/info) (user + channel/reason/message) (user + channel/cmd + reason) (channel + reason/topic/info) (user_modes + u_m_params) (user/channel/target/service + reason)
//	reason could be left empty to lower fct count

//	maybe split into different functions instead? (one for welcome, one for errorRepliews, one for channelMessages, etc)
*/