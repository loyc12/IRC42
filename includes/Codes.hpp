//#pragma once

#ifndef CODES_HPP
# define CODES_HPP

/* ---- REPLIES ---- */

// CLIENT-SERVER						000 - 099
# define	RPL_WELCOME					001 // "Welcome to the Internet Relay Network <nick>!<user>@<host>"
// # define	RPL_YOURHOST				002
// # define	RPL_CREATED					003
// # define	RPL_MYINFO					004
// # define	RPL_ISUPPORT				005
// # define	RPL_BOUNCE					010

// -- COMMAND RESPONSES					200 - 399
# define	RPL_UMODEIS					221 // "<user_modes> [<user_mode_params>]"
// # define	RPL_LUSERCLIENT				251
// # define	RPL_LUSEROP					252
// # define	RPL_LUSERUNKNOWN			253
// # define	RPL_LUSERCHANNELS			254
// # define	RPL_LUSERME					255
// # define RPL_ADMINME					256
// # define	RPL_ADMINLOC1				257
// # define	RPL_ADMINLOC2				258
// # define	RPL_ADMINEMAIL				259
// # define	RPL_TRYAGAIN				263
// # define	RPL_LOCALUSERS				265
// # define	RPL_GLOBALUSERS				266
// # define	RPL_WHOISCERTFP				276
// # define	RPL_NONE					300
# define	RPL_AWAY					301 // "<nick> :<message>"
// # define	RPL_USERHOST				302
// # define	RPL_UNAWAY					305
// # define	RPL_NOWAWAY					306
// # define	RPL_WHOREPLY				352
// # define	RPL_ENDOFWHO				315
// # define	RPL_WHOISREGNICK			307
// # define	RPL_WHOISUSER				311
// # define	RPL_WHOISSERVER				312
// # define	RPL_WHOISOPERATOR			313
// # define	RPL_WHOWASUSER				314
// # define	RPL_WHOISIDLE				317
// # define	RPL_ENDOFWHOIS				318
// # define	RPL_WHOISCHANNELS			319
// # define	RPL_WHOISSPECIAL			320
// # define	RPL_LISTSTART				321
// # define	RPL_LIST					322
// # define	RPL_LISTEND					323
# define	RPL_CHANNELMODEIS			324
// # define	RPL_CREATIONTIME			329
// # define	RPL_WHOISACCOUNT			330
# define	RPL_NOTOPIC					331 // "<channel> :<info>"
# define	RPL_TOPIC					332 // "<channel> :<topic>"
//# define	RPL_TOPICWHOTIME			333
// # define	RPL_INVITELIST				336
// # define	RPL_ENDOFINVITELIST			337
// # define	RPL_WHOISACTUALLY			338
# define	RPL_INVITING				341 // "<nick> <channel>"
// # define	RPL_INVEXLIST				346
// # define	RPL_ENDOFINVEXLIST			347
// # define	RPL_EXCEPTLIST				348
// # define	RPL_ENDOFEXCEPTLIST			349
// # define	RPL_VERSION					351
# define	RPL_NAMREPLY				353
// # define	RPL_LINKS					364
// # define	RPL_ENDOFLINKS				365
// # define	RPL_ENDOFNAMES				366
// # define	RPL_BANLIST					367
// # define	RPL_ENDOFBANLIST			368
// # define	RPL_ENDOFWHOWAS				369
// # define	RPL_INFO					371
// # define	RPL_ENDOFINFO				374
// # define	RPL_MOTD					372
// # define	RPL_MOTDSTART				375
// # define	RPL_ENDOFMOTD				376
// # define	RPL_WHOISHOST				378
// # define	RPL_WHOISMODES				379
# define	RPL_YOUREOPER				381 // ":<info>"
//# define	RPL_REHASHING				382
# define	RPL_TIME					391
// # define	RPL_USERSSTART				392
// # define	RPL_ENDOFUSERS  			394
// # define	RPL_NOUSERS  				395

// -- ERRORS							400 - 599
# define	ERR_UNKNOWNERROR			400
# define	ERR_NOSUCHNICK				401 // "<nick> :<reason>"
// # define	ERR_NOSUCHSERVER			402
# define	ERR_NOSUCHCHANNEL			403 // "<channel> :<reason>"
# define	ERR_CANNOTSENDTOCHAN		404 // "<channel> :<reason>"
# define	ERR_TOOMANYCHANNELS			405 // "<channel> :<reason>"
// # define	ERR_WASNOSUCHNICK			406
# define 	ERR_TOOMANYTARGETS			407 // "<target> :<reason>"
// # define	ERR_NOORIGIN				409
# define 	ERR_NORECIPIENT				411 // ":<reason>"
# define 	ERR_NOTEXTTOSEND			412 // ":<reason>"
# define	ERR_NOTOPLEVEL				413 // "<mask> :<reason>"
# define 	ERR_WILDTOPLEVEL			414 // "<mask> :<reason>"
// # define	ERR_INPUTTOOLONG			417
# define	ERR_UNKNOWNCOMMAND			421
// # define	ERR_NOMOTD					422
# define	ERR_NONICKNAMEGIVEN			431 // ":<reason>"
# define	ERR_ERRONEUSNICKNAME		432 // "<nick> :<reason>"
# define	ERR_NICKNAMEINUSE			433 // "<nick> :<reason>"
# define	ERR_NICKCOLLISION 			436 // "<nick> :<reason>"
# define 	ERR_UNAVAILRESOURCE 		437 // "<nick/channel/service> :<reason>"
# define	ERR_USERNOTINCHANNEL		441 // "<nick> <channel> :<reason>"
# define	ERR_NOTONCHANNEL			442 // "<channel> :<reason>"
# define 	ERR_USERONCHANNEL			443 // "<nick> <channel> [:<reason>]"
// # define ERR_USERDISABLED			446
# define	ERR_NOTREGISTERED			451
# define	ERR_NEEDMOREPARAMS			461 // "<client> <command> :Not enough parameters"
# define	ERR_ALREADYREGISTERED		462 // ":<reason>"
# define	ERR_PASSWDMISMATCH			464 // ":<reason>"
// # define	ERR_YOUREBANNEDCREEP		465
# define 	ERR_KEYSET					467 // "<channel> :<reason>"
# define	ERR_CHANNELISFULL			471 // "<channel> :<reason>"
# define	ERR_UNKNOWNMODE				472
# define	ERR_INVITEONLYCHAN			473 // "<channel> :<reason>"
# define	ERR_BANNEDFROMCHAN			474 // "<channel> :<reason>"
# define	ERR_BADCHANNELKEY			475 // "<channel> :<reason>"
# define	ERR_BADCHANMASK				476 // "<channel> :<reason>"
# define	ERR_NOPRIVILEGES			481 // ":Permission Denied- You're not an IRC operator"
# define	ERR_CHANOPRIVSNEEDED		482 // "<channel> :<reason>"
// # define	ERR_CANTKILLSERVER			483
# define 	ERR_RESTRICTED 				484 // ":<reason>"
# define	ERR_NOOPERHOST				491 // ":<reason>"
# define	ERR_UMODEUNKNOWNFLAG		501 // ":<reason>"
# define	ERR_USERSDONTMATCH			502 // ":<reason>"
// # define	ERR_HELPNOTFOUND			524
// # define	ERR_INVALIDKEY				525

// -- SSL / TLS							600 - 699
// # define	RPL_STARTTLS				670
// # define	RPL_WHOISSECURE				671
// # define	ERR_STARTTLS				691
// # define	ERR_INVALIDMODEPARAM		696

// -- HELP								700 - 799
// # define	RPL_HELPSTART				704
// # define	RPL_HELPTXT					705
// # define	RPL_ENDOFHELP				706
// # define	ERR_NOPRIVS					723

// -- LOGIN and SASL
// # define	RPL_LOGGEDIN				900
// # define	RPL_LOGGEDOUT				901
// # define	ERR_NICKLOCKED				902
// # define RPL_SASLSUCCESS				903
// # define ERR_SASLFAIL				904
// # define ERR_SASLTOOLONG				905
// # define ERR_SASLABORTED				906
// # define ERR_SASLALREADY				907
// # define RPL_SASLMECHS				908

#endif // CODES_HPP