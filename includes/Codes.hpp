//#pragma once

#ifndef CODES_HPP
# define CODES_HPP

/* ---- REPLIES ---- */

// CLIENT-SERVER						000 - 099
# define	RPL_WELCOME					"001" // "Welcome to the Internet Relay Network <nick>!<user>@<host>"

// -- COMMAND RESPONSES					200 - 399
 # define	RPL_USERHOST				"302"
# define	RPL_NOTOPIC					"331" // "<channel> :<info>"
# define	RPL_TOPIC					"332" // "<channel> :<topic>"
# define	RPL_NAMREPLY				"353"
# define	RPL_ENDOFNAMES				366

// -- ERRORS							400 - 599
# define	ERR_NOSUCHNICK				401 // "<nick> :<reason>"
# define	ERR_NOSUCHCHANNEL			"403" // "<channel> :<reason>"
# define	ERR_CANNOTSENDTOCHAN		404 // "<channel> :<reason>"
# define	ERR_UNKNOWNCOMMAND			421
# define	ERR_ERRONEUSNICKNAME		"432" // "<nick> :<reason>"
# define	ERR_NICKNAMEINUSE			"433" // "<nick> :<reason>"
# define	ERR_NEEDMOREPARAMS			"461" // "<client> <command> :Not enough parameters"
# define	ERR_ALREADYREGISTERED		"462" // ":<reason>"
# define	ERR_PASSWDMISMATCH			"464" // ":<reason>"
# define	ERR_CHANNELISFULL			"471" // "<channel> :<reason>"
# define	ERR_INVITEONLYCHAN			"473" // "<channel> :<reason>"
# define	ERR_NOPRIVILEGES			481 // ":Permission Denied- You're not an IRC operator"
# define	ERR_CHANOPRIVSNEEDED		482 // "<channel> :<reason>"
# define	ERR_UMODEUNKNOWNFLAG		501 // ":<reason>"


#endif // CODES_HPP