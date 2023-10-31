# ft_irc
Server to host a chat.

## Netcat:
* Everything happens in the terminal. No graphic interface
* Check if you have Netcat. Terminal: nc. To install: brew install netcat
* To connect: nc localhost <port>
* NOTE : localhost -> system preference/network -> IP adress

HOW TO WRITE THE COMMAND IN NETCAT. Don't forget CTRL-VM before ENTER
PASS <pass> CTRL-VM before ENTER
USER <username> * * <nickname> CTRL-VM before ENTER
NICK <nickname> CTRL-VM before ENTER

JOIN #chanName CTRL-VM before ENTER
PART #chanName CTRL-VM before ENTER ->quit a channel
QUIT CTRL-VM before ENTER ->quit the server

PRIVMSG <#chanName> :<message> CTRL-VM before ENTER
PRIVMSG <nickname> :<message> CTRL-VM before ENTER

INVITE <nickname> <#chanName> CTRL-VM before ENTER
KICK #chanName <nickname> CTRL-VM before ENTER

TOPIC <#chanName> <topicToSet> CTRL-VM before ENTER
TOPIC <#chanName> CTRL-VM before ENTER **so everyone can know the topic of the channel at anytime.

MODE:
MODE <#chanName> +i CTRL-VM before ENTER ->to set the channel to INVITE ONLY
MODE <#chanName> +t CTRL-VM before ENTER ->to protect the topic of the channel. So no one changes it
MODE <#chanName> -t CTRL-VM before ENTER ->to unprotect the topic of the channel
MODE <#chanName> +k <password> CTRL-VM before ENTER ->to set a password with the chan
MODE <#chanName> -k <password> CTRL-VM before ENTER ->to unset the password of the chan
MODE <#chanName> +o <nickname> CTRL-VM before ENTER ->to give privileges of chanOp to someone
MODE <#chanName> -o <nickname> CTRL-VM before ENTER ->to take those privileges away
MODE <#chanName> +l <numberOfMembersToSet> CTRL-VM before ENTER ->to set a limit of members to a channel
MODE <#chanName> -l CTRL-VM before ENTER  ->to unset the limit of members for a chan

## Limechat
Make sure that ALL the command are typed in the log window(where is the "conversation" with the server)


At School ---> Get LimeChat IRC Client
Ouvre Limechat-> server/serverproperties/

Network name : irc42
server : localhost
Port: 6666

Nickname : al
Login: al or whatever you want
Real name : al or whatever you want

Dans le terminal (apres le make) : ./ircserv 6667 1234


## TODO (LL)
	check NOTE (LL) in server_cmds: if statements are wonkey and self defeating. spaghet???
	joining and leaving are iffy (concerned user is not properly notified : ghost users)
	look for invalid reads when calling NICK
	rework replies :
		RPL_TOPIC
		RPL_NOTOPIC ???
		RPL_INVITING ???

	inform everyone on nickname change (?)
	only execute the first line in the user buffer