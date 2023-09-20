#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <string>

/*
char	**splitString(char *str, char *chrs);
char	**splitString(char *str, std::string chrs);
char	**splitString(std::string str, char *chrs);
char	**splitString(std::string str, std::string chrs);
*/

std::string	*splitString(const char *str, const char *chrs);
std::string	*splitString(const char *str, const std::string chrs);
std::string	*splitString(const std::string str, const char *chrs);
std::string	*splitString(const std::string str, const std::string chrs);

#endif // UTILS_HPP