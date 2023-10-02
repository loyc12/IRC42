#include "IRC.hpp"

void	debugPrint(std::string color, std::string message)	{std::cout << color << message << DEFCOL;}

std::string	*splitStringPrivate(const char *str, const char *chrs)
{
//	std::cout << "buff : \'" << str << "\'" << std::endl; //							DEBUG
//	std::cout << "chars : \'" << chrs << "\'" << std::endl; //							DEBUG

	int		i = 0;
	char	*ptr = strtok(strdup(str), chrs); //	strtok works iteratively ..
	while (ptr != NULL)
	{
		ptr = strtok(NULL, chrs); //			... so it needs to be once per token
		i++;
	}

	std::string *args = new std::string[i];

	i = 0;
	ptr = strtok(strdup(str), chrs);
	while (ptr != NULL)
	{
		args[i] = strdup(ptr);
//		std::cout << "args[" << i << "] : \'" << args[i] << "\'" << std::endl; //		DEBUG
		ptr = strtok(NULL, chrs);
		i++;
	}

	return args;
}
std::string	*splitString(const char *str, const char *chrs)
{
	return splitStringPrivate(str, chrs);
}
std::string	*splitString(const char *str, const std::string chrs)
{
	return splitStringPrivate(str, chrs.c_str());
}
std::string	*splitString(const std::string str, const char *chrs)
{
	return splitStringPrivate(str.c_str(), chrs);
}
std::string	*splitString(const std::string str, std::string const chrs)
{
	return splitStringPrivate(str.c_str(), chrs.c_str());
}