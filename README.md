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

## Problem:
* Dans Netcat, quand je fais CTRL-C, error at rcv() Connection reset by peer -> Est-ce qu'il faut faire un peu comme on a fait quand on avait un prob at select()??