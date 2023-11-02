#include "IRC.hpp"

void	debugPrint(std::string color, std::string message)	{std::cout << color << message << DEFCOL << std::endl;}

//			SPLITS A STRING INTO A VECTOR OF STRING TOKENS, USING CHARS AS DELIMIERS
std::vector<std::string>	splitString(std::string str, const char *dlmtrs)
{
	std::vector<std::string> args;


	char	*ptr = strtok((char *)str.c_str(), dlmtrs);

//	NOTE : strtok works iteratively, so it needs to be called once per token
	while (ptr != nullptr && !std::string(ptr).empty())
	{
		args.push_back(std::string(ptr));
		ptr = strtok(NULL, dlmtrs);
	}

	return args;
}

std::string	makeChanMsg(User *user, std::string input)
{
	std::ostringstream 	message;
	message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << input << "\r\n";
	return (message.str());
}
std::string	makeChanMsg(User *user, std::string code, std::string input)
{
	std::ostringstream 	message;
	message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << code << " " << input << "\r\n";
	return (message.str());
}

std::string	makeUserMsg(User *user, std::string input)
{
	std::ostringstream 	message;
	message << ":" << user->getHostname() << " " << user->getNick() << " :" << input << "\r\n";
	return (message.str());
}
std::string	makeUserMsg(User *user, std::string code, std::string input)
{
	std::ostringstream 	message;
	message << ":" << user->getHostname() << " " << code << " " << user->getNick() << " :" << input << "\r\n";
	return (message.str());
}

std::string	makePrivMsg(User *sender, std::string input)
{
	std::ostringstream 	message;
	message << ":" << sender->getNick() << " " << input << "\r\n";
	return (message.str());
}

bool isSameUser(User* user1, User* user2)
{
	if ((void *)user1 == (void *)user2)
		return (true);
	return (false);
}

//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	sendToUser(User *targetUser, std::string message)
{
	std::ostringstream debug; //															DEBUG
	debug << "OUTGOING USER_MSG TO : " << targetUser->getNick() << " :\n" << message; //	DEBUG
	debugPrint(GREEN, debug.str()); //														DEBUG

	if (send(targetUser->getFD(), message.c_str(), message.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToUser() ");
}

//		DEBUG
void	printChars(std::string str)
{
	std::cerr << "message values : " << std::endl;
	for (int i = 0; i < (int)str.length(); i++)
		std::cerr << "[" << (int)str[i] << "] ";
	std::cerr << std::endl;
}
