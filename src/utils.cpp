#include "IRC.hpp"

void	debugPrint(std::string color, std::string message)	{std::cout << color << message << DEFCOL << std::endl;}

//							SPLITS A STRING INTO A VECTOR OF STRING TOKENS, USING CHARS AS DELIMIERS
std::vector<std::string>	splitStringPrivate(std::string str, const char *dlmtrs)
{
	std::vector<std::string> args;


	char	*ptr = strtok((char *)str.c_str(), dlmtrs);

//	NOTE : strtok works iteratively, so it needs to be called once per token
	while (ptr != nullptr && std::string(ptr).compare("") != 0)
	{
		args.push_back(std::string(ptr));
		std::cout << ptr << std::endl; //					DEBUG
		ptr = strtok(NULL, dlmtrs);
	}

	return args;
}

// std::vector<std::string>	splitStringPrivate(std::string str, std::string dlmtrs)
// {
// 	std::vector<std::string> args;

// 	// char	*ptr = strtok((char *)str.c_str(), dlmtrs);
// 	size_t	start = str.find_first_not_of(dlmtrs), end=start;

// 	while (start != std::string::npos)
// 	{
// 		end = str.find(dlmtrs, start);
// 		args.push_back(str.substr(start, end-start));
// 		start = str.find_first_not_of(dlmtrs, end);
// 	}
// 	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
// 		std::cout << *it << std::endl;

// 	return args;
// }


//							PUBLIC OVERLOADS OF splitStringPrivate THAT TAKES COMBINATIONS OF C_STRs and STD::STRINGs
// std::vector<std::string>	splitString(const char *str, const char *chrs)				{ return splitStringPrivate(std::string(str), chrs); }
// std::vector<std::string>	splitString(const char *str, const std::string chrs)		{ return splitStringPrivate(std::string(str), chrs.c_str()); }
std::vector<std::string>	splitString(const std::string str, const char *chrs)		{ return splitStringPrivate(str, chrs); }
//std::vector<std::string>	splitString(const std::string str, std::string const chrs)	{ return splitStringPrivate(str, chrs.c_str()); }
//std::vector<std::string>	splitString(const std::string str, std::string const chrs)	{ return splitStringPrivate(str, chrs); }

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
	debug << "OUTGOING USER_MSG TO : " << targetUser->getNick() << " :\n" << message; //	DEBUG
	debugPrint(GREEN, debug.str()); //														DEBUG

	if (send(targetUser->getFD(), message.c_str(), message.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToUser() ");
}

void	tellChanMode(User *user, Channel *chan, std::string mode)
{
	sendToUser(user, makeChanMsg(user, "MODE " + chan->getChanName(), mode));
}

//		DEBUG
void	printChars(std::string str)
{
	std::cerr << "message values : " << std::endl;
	for (int i = 0; i < (int)str.length(); i++)
		std::cerr << "[" << (int)str[i] << "] ";
	std::cerr << std::endl;
}