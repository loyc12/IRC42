#include "IRC.hpp"

void	debugPrint(std::string color, std::string message)	{std::cout << color << message << DEFCOL << std::endl;}


std::vector<std::string>	splitStringPrivate(const char *str, const char *chrs) //					NOTE : make all splitStrings return a vector of strings instead
{
//	std::cout << "buff : \'" << str << "\'" << std::endl; //							DEBUG
//	std::cout << "chars : \'" << chrs << "\'" << std::endl; //							DEBUG

/* //TODO can it be deleted?? below?
	while (ptr != NULL)

	{
		ptr = strtok(NULL, chrs); //			... so it needs to be once per token
		i++;
	}
*/

//	std::string *args = new std::string[i]; //container vector

	std::vector<std::string> args;

	int		i = 0;
	char	*ptr = strtok(strdup(str), chrs); //	strtok works iteratively ..
	ptr = strtok(strdup(str), chrs);
	while (ptr != NULL)
	{
		args.push_back(std::string(strdup(ptr)));
//		std::cout << "args[" << i << "] : \'" << args[i] << "\'" << std::endl; //		DEBUG
		ptr = strtok(NULL, chrs);
		i++;
	}

	return args;
}
std::vector<std::string>	splitString(const char *str, const char *chrs)
{
	return splitStringPrivate(str, chrs);
}
std::vector<std::string>	splitString(const char *str, const std::string chrs)
{
	return splitStringPrivate(str, chrs.c_str());
}
std::vector<std::string>	splitString(const std::string str, const char *chrs)
{
	return splitStringPrivate(str.c_str(), chrs);
}
std::vector<std::string>	splitString(const std::string str, std::string const chrs)
{
	return splitStringPrivate(str.c_str(), chrs.c_str());
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



//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	sendToUser(User* targetUser, std::string message)
{
	std::ostringstream debug; //															DEBUG
	debug << "OUTGOING USER_MSG TO : (" << targetUser->getFD() << ")\t| " << message; //	DEBUG
	debugPrint(GREEN, debug.str()); //														DEBUG

	if (send(targetUser->getFD(), message.c_str(), message.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToUser() ");
}
