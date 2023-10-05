#ifndef UTILS_HPP
# define UTILS_HPP

# include "IRC.hpp"

void 	debugPrint(std::string color, std::string message);

std::vector<std::string>	splitString(const char *str, const char *chrs);
std::vector<std::string>	splitString(const char *str, const std::string chrs);
std::vector<std::string>	splitString(const std::string str, const char *chrs);
std::vector<std::string>	splitString(const std::string str, const std::string chrs);

#endif // UTILS_HPP
