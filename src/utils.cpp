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
