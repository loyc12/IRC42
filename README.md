# IRC42

//port un peu comme un fd
TESTTESTTEST

## Installation

At School ---> Get LimeChat IRC Client
Ouvre Limechat-> server/serverproperties/

Network name : irc42
server : localhost
Port: 6667

Nickname : al
Login: al
Real name : al

OK

Click Droit --> Connect ( Va dire que la connection est refusee)
Dans le terminal (apres le make) : ./ircserv 6667 1234

## Devoir

* Envoyer et recevoir message

## Note :

* Eviter de prendre des rencontres de Jeudi


## Netcat:
* tout se passe dans le terminal pour netcat. Pas d'interface graphique
* Il faut s'assurer de l'installer sur notre appareil. Dans le terminal: brew install netcat
* Après la ligne de commande pour se connecter: netcat localhost <port>
* Il faut que je regarde pour le mot de passe, j'imagine que la logique serait de l'ajouter à la suite de port...

##to send welcome msg:
/*oss << ":" << m_hostname << " 001 " << m_userDB[fd].m_nickname << " :Welcome to the IRCServ, " << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username << "@" << m_hostname << "\r\n";*/
		std::ostringstream ss;
		ss << GREEN << "Welcome to this IRC server!" << NOCOLOR << "\r\n";
		std::string welcome = ss.str();

		ret = send(fd, welcome.c_str(), welcome.size(), 0);

/*--- Trying to find a solution for channel ----*/
The numeric code or status code that you would need to add to your JOIN message depends on the specific protocol or messaging system you are using. In the provided code snippet, it appears that you are constructing an IRC (Internet Relay Chat) JOIN message.

In IRC, there are no specific numeric codes or status codes that need to be included in the JOIN message itself. Instead, the server will send a response to the JOIN message indicating the success or failure of the join operation. The response will include a numeric code and a message.

For example, if the join operation is successful, the server may respond with a numeric code 332 (RPL_TOPIC) indicating the topic of the channel, followed by a numeric code 353 (RPL_NAMREPLY) indicating the list of users in the channel.

If the join operation fails, the server may respond with a numeric code 403 (ERR_NOSUCHCHANNEL) indicating that the channel does not exist or a numeric code 471 (ERR_CHANNELISFULL) indicating that the channel is full.

You would need to handle these response codes in your server code to appropriately handle the join operation.
