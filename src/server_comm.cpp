#include "IRC.hpp"

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user)
{
	sendToUser(user, makeUserMsg(user, RPL_WELCOME, WELCOME_HEADER));
	user->wasWelcomed = true;
}


void	Server::readFromClient(User *user, int fd)
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
		std::ostringstream debug; //										DEBUG
		debug << "INCOMING USER_MSG FROM (" << fd << ") :\n" << buff; //	DEBUG
		debugPrint(BLUE, debug.str()); //									DEBUG

		std::string str;
    	user->lastMsg.append(str.assign(buff, 0, byteReceived));

		std::vector<std::string> args = splitString(buff, " \r\n");

		if (isMsgEnd(user->lastMsg))
		{
			if (execCommand(user, args))
				sendToUser(user, makeUserMsg(user, ERR_UNKNOWNCOMMAND, "invalid command"));
			user->lastMsg = ""; //			NOTE : reset user's msg buffer
		}
	}
	bzero(buff, BUFFSIZE);
}
