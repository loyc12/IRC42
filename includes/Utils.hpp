#ifndef UTILS_HPP
# define UTILS_HPP

# include "IRC.hpp"

void 	debugPrint(std::string color, std::string message);

std::vector<std::string>	splitString(const std::string str, const char *chrs);
std::string					makeChanMsg(User *user, std::string input);
std::string					makeChanMsg(User *user, std::string code, std::string input);
std::string					makeUserMsg(User *user, std::string input);
std::string					makeUserMsg(User *user, std::string code, std::string input);
std::string					makePrivMsg(User *sender, std::string input);

bool						isSameUser(User *user1, User *user2);

void						sendToUser(User *targetUser, std::string message);

void						printChars(std::string str); //		DEBUG

#endif // UTILS_HPP
