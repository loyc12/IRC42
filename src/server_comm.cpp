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

		debugPrint(RED, args[0]); // 											DEBUG

		std::ostringstream debug; //											DEBUG
		debug << "INCOMING USER_MSG FROM (" << fd << ") :\n" << *lastMsg; //	DEBUG
		debugPrint(BLUE, debug.str()); //										DEBUG

		if (execCommand(user, args) == -1)
		{

			//	if is a channel message : send to channel users
			if (args[0].compare("PRIVMSG") == 0)
				processChanMsg(user, *lastMsg, args);
			else
        		sendToUser(user, makeUserMsg(user, *lastMsg));
		}
	}
	bzero(buff, BUFFSIZE);
}
