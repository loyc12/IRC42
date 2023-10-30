# ft_irc
Server to host a chat.

## Netcat:
* Everything happens in the terminal. No graphic interface
* Check if you have Netcat. Terminal: nc. To install: brew install netcat
* To connect: nc localhost <port>
* NOTE : localhost -> system preference/network -> IP adress

PASS <pass> CTRL-VM before ENTER
USER <username> * * <nickname> CTRL-VM before ENTER
NICK <nickname> CTRL-VM before ENTER

PRIVMSG #chanName :<message> CTRL-VM before ENTER
PRIVMSG nickname :<message> CTRL-VM before ENTER

JOIN #chanName CTRL-VM before ENTER
PART #chanName CTRL-VM before ENTER ->quit a channel
QUIT CTRL-VM before ENTER ->quit the server
KICK #chanName <nickname> CTRL-VM before ENTER

## Limechat

At School ---> Get LimeChat IRC Client
Ouvre Limechat-> server/serverproperties/

Network name : irc42
server : localhost
Port: 6666

Nickname : al
Login: al
Real name : al

Dans le terminal (apres le make) : ./ircserv 6667 1234
