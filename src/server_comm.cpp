#include "IRC.hpp"

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user)
{
	sendToUser(user, makeUserMsg(user, RPL_WELCOME, WELCOME_HEADER));
	user->setWelcome(true);
}


void	Server::readFromClient(User *user, int fd)
{
	if (shutoff)
	{
		std::cerr << "Is shut" << std::endl;
		return;
	}

	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Handles what to do depending on the byte value (error, null or message)
	if (byteReceived == 0)
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
		str.assign(buff, 0, byteReceived);
    	user->setLastMsg(str.append(user->getLastMsg()));

		debugPrint(YELLOW, user->getLastMsg());

		if (isMsgEnd(user->getLastMsg()))
		{
			std::vector<std::string> args = splitString(user->getLastMsg(), " \r\n");
			if (execCommand(user, args))
				sendToUser(user, makeUserMsg(user, "ERR_UNKNOWNCOMMAND", "invalid command"));

			user->setLastMsg("");
			if (user->wasWelcomed() == false && user->getLoginStep() == 0b111) // aka user finished login in
				welcomeUser(user);
		}
		else
		{
			debugPrint(YELLOW, "!isMsgEnd(user->lastMsg)");
		}
	}
	bzero(buff, BUFFSIZE);
}
