#include "IRC.hpp"

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user)
{
	sendToUser(user, makeUserMsg(user, RPL_WELCOME, WELCOME_HEADER));
	user->setWelcome(true);
}

void	Server::readFromClient(User *user, int fd)
{
	char	buff[BUFFSIZE];
	bzero (buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Handles what to do depending on the byte value (error, null or message)
	if (byteReceived == 0) //		Deletes the client, loses its FD and removes it from the baseFds
		deleteClient(fd);
	else if (byteReceived > 0)
	{
		if (!hasClient(user) || this->isShutOff())
			return;

		std::ostringstream debug; //													DEBUG
		debug << "INCOMING USER_MSG FROM : " << user->getNick() << " :\n" << buff; //	DEBUG
		debugPrint(BLUE, debug.str()); //												DEBUG

		std::string str;
		str.assign(buff, 0, byteReceived);
    	user->setLastMsg(user->getLastMsg().append(str));
		//	NOTE : when using valgrind, delays allows multiple messages to be treated at once
		if (isMsgEnd(user->getLastMsg()))
		{
		//	printChars(user->getLastMsg()); //											DEBUG

			std::vector<std::string> args = splitString(user->getLastMsg(), " \r\n");

			if (execCommand(user, args))
				sendToUser(user, makeUserMsg(user, ERR_UNKNOWNCOMMAND, "Invalid command"));

			if (!hasClient(user) || this->isShutOff())
				return;

			user->setLastMsg("");
			if (!user->wasWelcomed() && user->isLoggedIn())
				welcomeUser(user);
		}
		else
			debugPrint(YELLOW, "received a partial message..."); //						DEBUG
	}
	bzero(buff, BUFFSIZE);
}

void	Server::sendToServ(std::string message)
{
	for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		sendToUser(it->second, message);
}
