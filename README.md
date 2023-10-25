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

*

## Note :

* Eviter de prendre des rencontres de Jeudi


## Netcat:
* tout se passe dans le terminal pour netcat. Pas d'interface graphique
* Il faut s'assurer de l'installer sur notre appareil. Dans le terminal: brew install netcat
* Après la ligne de commande pour se connecter: netcat localhost <port>
* Il faut que je regarde pour le mot de passe, j'imagine que la logique serait de l'ajouter à la suite de port...
PASS <pass>ctrlVM après enter
NICK <nick>ctrlVM après enter
USER <username> * * :<nick>ctrlVM après enter

PRIVMSG #chanName :<le message>ctrlVM après enter
PRIVMSG <nickname> :<le message>ctrlVM après enter

JOIN #chanNamectrlVM après enter
PART #chanNamectrlVM après enter ->quitter un channel
QUITctrlVM ->quitte le server

## REVIEW

checklist :
TODO :
	* test duplicate nicknames
	* define code : trouver, et les entrer dans le hpp, enlever excedent
	* les notes doivent etre ecrites toutes de la meme facon
	* debug comment out + push github
	* Relire le PDF au complet
	* TCP IP (info): on utiliser v4; 32-bits adresse; on a choisi -> moins complexe, plus facile, mais il a des limites sur le nombre de IP possible à créer
	* netcat moi et loic
	
