//#pragma once

#ifndef CODES_HPP
# define CODES_HPP

/* ---- REPLIES ---- */

//ENTRY CODE


// CLIENT-SERVER						000 - 099
# define	RPL_WELCOME					"001" // "Welcome to the Internet Relay Network <nick>!<user>@<host>" YES

// -- COMMAND RESPONSES					200 - 399
 # define	RPL_USERHOST				"302" //YES
# define	RPL_NOTOPIC					"331" // "<channel> :<info>" YES
# define	RPL_TOPIC					"332" // "<channel> :<topic>" YES
# define	RPL_NAMREPLY				"353"// YES
# define	RPL_ENDOFNAMES				366// YES

// -- ERRORS							400 - 599
# define	ERR_NOSUCHNICK				401 // "<nick> :<reason>" YES
# define	ERR_NOSUCHCHANNEL			"403" // "<channel> :<reason>" YES
# define	ERR_CANNOTSENDTOCHAN		404 // "<channel> :<reason>" YES
# define	ERR_UNKNOWNCOMMAND			421 // YES
# define	ERR_ERRONEUSNICKNAME		"432" // "<nick> :<reason>" YES
# define	ERR_NICKNAMEINUSE			"433" // "<nick> :<reason>" YES
# define	ERR_NEEDMOREPARAMS			"461" // "<client> <command> :Not enough parameters" YES
# define	ERR_ALREADYREGISTERED		"462" // ":<reason>" YES
# define	ERR_PASSWDMISMATCH			"464" // ":<reason>" YES
# define	ERR_CHANNELISFULL			"471" // "<channel> :<reason>" YES
# define	ERR_INVITEONLYCHAN			"473" // "<channel> :<reason>" YES
# define	ERR_NOPRIVILEGES			481 // ":Permission Denied- You're not an IRC operator" YES
# define	ERR_CHANOPRIVSNEEDED		482 // "<channel> :<reason>" YES
# define	ERR_UMODEUNKNOWNFLAG		501 // ":<reason>" YES


#endif // CODES_HPP