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

## Note :
* Eviter de prendre des rencontres de Jeudi

## Netcat:
* tout se passe dans le terminal pour netcat. Pas d'interface graphique
* Il faut s'assurer de l'installer sur notre appareil. Dans le terminal: brew install netcat
* Après la ligne de commande pour se connecter: nc localhost <port> (us, we use 6666 for memo reasons)
* NOTE : localhost -> system preference/network -> IP adress

PASS <pass>
NICK <nick>
USER <username>

PRIVMSG #chanName :<le message>
PRIVMSG nickname :<le message>

JOIN #chanName
PART #chanName ->quitter un channel
QUIT ->quitte le server

## REVIEW

checklist :
TODO :
	* checkpass = implement in boool pour eviter de faire lire quelque chose qui existe pas
	* readfromclient -> lit de la donnee qui existe pas, quand client disconnecte.... a suivre
	* Relire le PDF au complet
	* TCP IP (info): on utiliser v4; 32-bits adresse; on a choisi -> moins complexe, plus facile, mais il a des limites sur le nombre de IP possible à créer
	* netcat moi et loic
	* memory leaks also