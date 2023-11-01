#ifndef UTILS_HPP
# define UTILS_HPP

# include "IRC.hpp"

void 	debugPrint(std::string color, std::string message);

// std::vector<std::string>	splitString(const char *str, const char *chrs);
// std::vector<std::string>	splitString(const char *str, const std::string chrs);
std::vector<std::string>	splitString(const std::string str, const char *chrs);
//std::vector<std::string>	splitString(const std::string str, const std::string chrs);

std::string					makeChanMsg(User *user, std::string input);
std::string					makeChanMsg(User *user, std::string code, std::string input);
std::string					makeUserMsg(User *user, std::string input);
std::string					makeUserMsg(User *user, std::string code, std::string input);
std::string					makePrivMsg(User *sender, std::string input);

void						sendToUser(User *targetUser, std::string message);
void						tellChanMode(User *user, Channel *chan, std::string mode);

void						printChars(std::string str);

#endif // UTILS_HPP
