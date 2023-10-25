#include "IRC.hpp"

void	debugPrint(std::string color, std::string message)	{std::cout << color << message << DEFCOL << std::endl;}



//							SPLITS A STRING INTO A VECTOR OF STRINGS, USING CHARS AS DELIMIERS
std::vector<std::string>	splitStringPrivate(std::string str, const char *dlmtrs)
{
	std::vector<std::string> args;

	char	*ptr = strtok((char *)str.c_str(), dlmtrs);

//	NOTE : strtok works iteratively, so it needs to be called once per token
	while (ptr != NULL)
	{
		std::cerr << std::string(ptr) << "." << std::endl; //								DEBUG

		args.push_back(std::string(ptr));

		ptr = strtok(NULL, dlmtrs);
	}

	return args;
}



//							PUBLIC OVERLOADS OF splitStringPrivate THAT TAKES COMBINATIONS OF C_STRs and STD::STRINGs
std::vector<std::string>	splitString(const char *str, const char *chrs)				{ return splitStringPrivate(std::string(str), chrs); }
std::vector<std::string>	splitString(const char *str, const std::string chrs)		{ return splitStringPrivate(std::string(str), chrs.c_str()); }
std::vector<std::string>	splitString(const std::string str, const char *chrs)		{ return splitStringPrivate(str, chrs); }
std::vector<std::string>	splitString(const std::string str, std::string const chrs)	{ return splitStringPrivate(str, chrs.c_str()); }

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


//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	sendToUser(User *targetUser, std::string message)
{
	std::ostringstream debug; //															DEBUG
	debug << "OUTGOING USER_MSG TO : (" << targetUser->getFD() << ")\t| " << message; //	DEBUG
	debugPrint(GREEN, debug.str()); //														DEBUG

	if (send(targetUser->getFD(), message.c_str(), message.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToUser() ");
}
