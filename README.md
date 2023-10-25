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


## Problem:
* Quand on crée un channel -> on devient OWNER du channel
* Quand le owner revient dans son channel, il doit récupérer chanop.
* Quand on est un chanop et on quitte le channel -> il faut laisser les privilèges de op à un membre qui est le plus ancien dans le channel **à tester
* pour la liste des membres actifs d'un channel; peut-être mieux d'avoir une map ou pair, en User* et son code (membre régulier, OWNER, chanop)


* Netcat: quand un client envoie un message. Sur Limechat, le message arrive seulement avec le premier arg
* Dans Netcat, il peut rejoindre un channel "vide". ligne de commande: JOIN
suivi de rien d'autre. Il devient un channel
* il faut que la liste des participants soient mis à jour pour ceux qui sont déjà dedans le channel. Pour le moment, non mis à jour.

* Weird -> j'ai créé un chan sans ajouter quoique ce soit, il est devenu invite only... Not everytime
* Need to update the list of participants in channel when people enter channel... Maybe everyone needs the notice when someone was added to the channel to update the list... ??


## VAL: ----------------------------------------------------------------------------------------------------------------------------------------------------------------


## -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

std::string	ftMessage(std::string code)
{
	//pointeur sur element de string (message a envoyer)
}
command(int target, User *user, std::string condition)
{
	std::string *code;
	int	ret;

	if (target == CHAN)
	{
		if (conditon == MODE)
		{
			//fonction qui regarde attributMode et retourne une valeur positive ou negative selon le mode mit en place && cette fonction effectue une modification sur le code.
			ret = attributMode(user, &code);
			//On veut regarder les attributs mode du channel
			if (ret < 0)
				replyTo(REQUEST, user, *code, ftMessage(*code));
			else
			{
			// WE ARE HERE
				ftAction();
				reply(REQUEST, );
				replyTo(CHAN, user, *code, ftMessage(*code));
			}
		}
		else if (condition == USER)
		{
			//On veut regarder les attributs mode du channel
			if (attributUser(user, &code) < 0)
				replyTo(REQUEST, user, *code, ftMessage(*code));
		}
	}
	else if (target == REQUEST)
	{

	}
	//	TODO:  TRIGGER : Command ID
}

/*
	IN REQUEST :
		You can :
				PASS (si commande pass : envoyer un erreur ERR_ALREADYREGISTRE....),
				NICK (CHAN ET REQUEST (if true && CHAN = envoyer message a tous) if false ( channel meme nickname ) = ERR_432, refus (il doit changer son username avant)) message a tous si dans chan
				JOIN (trigger : VERIF NICKNAME, ATTRIBUTMODE -> if false = retourner un message d'erreur a base socket) Si new chan = createur = -o (operateur)
					exemple : password channel false - > retourne BAD CHANNEL KEY 475))
					(limit -l) Limite utilisateurs par canal
		You can't :
				MESSAGE (du channel, trigger participants du channel (QT)),
					command(KICK, CHAN, conditon : MODE,  false -> message to baseSocket (482), if true -> kick  + MESSAGE),
					MODE(CHAN, condition : USER (operateur), false -> message to baseSocket (-i, -t, -k, -l), -o), if true ->action + MESSAGE),
					INVITE(CHAN, condition : mode, if false ---> message to baseSocket, (-i), if true -> action + message),
					TOPIC(CHAN, condition : mode, if false--> message to baseSocket (-t), , if true -> action + message)
	IN CHAN :
		You can :
		You can't :
*/


//KICK(CHAN, conditon : MODE,  false -> message to Requesr (482), if true -> kick  + MESSAGE)

// void	kick(int fd)//client user :
// {
// 	//not in a channel
// 	if (this->_baseFds == fd)
// 		sendTo(REQUEST, CODE_PAS_A_BONNE_PLACE); // Envoie le message dans Request pour dire que tu peux pas faire la commande la
// 	else
// 	{
// 			// fd est un chan, pour l'instant on ne sait pas lequel
// 			/*
// 			loop a travers
// 			*/
// 	}
// 	//kick dans quel channel

// }

// BEFORE CALLING FUNCTIONS : cheks if channel and users exist

// 	if (chann == NULL)
// 		send deny message (not on channel)
// 	else if (functionLacksParams(user, args))
// 		send deny message (not enough params)
// 	else if (functionIsNotAllowed(user, chan))
// 		send deny message (not allowed)
// 	else
// 		send aprove message (success)
// 		kick


 Pogner un asterix ?


//On fait un chan operator que son pouvoir, c'est d'autoriser ou refuser un invite only.


## LOYC :

TODO :
	* finish implementing setChanMode()
	* prevent users to connect to serv with duplicate nicknames (and not just to chan)
	* check for user presence and perms when calling channel functions
	* when a chanOp leaves the channel and there is no other chanOp, needs to give chanOp to oldest in the channel


## ALEX:

demande :
* standariser les codes a envoyer au client ?

checklist :
	* JOIN #1 dhjfkjsdh -> error 403 envoie un reply, pas un code derreur
	* MODE #CHANNL +o, send message Informer chanop qui est op : DONE
	* PRIVMSG USER a implementer
	* TCP IP (info)
	* netcat moi et loic

- - - - - 
	* test de channel :
	* MODE #channel !i (X)
	* MODE #channel (X)

	* JOIN (X) -> ok
	* TOPIC #chan -> ok

	* JOIN 1 (<hostadress> has changed mode: o alex) -> po sur

	* TOPIC (x) Reply(403): channel does not exist -> to change
	* TOPIC (#chanexistepas): channel does not exist -> to change
	* TOPIC (channelexistemaispas#): channel does not exist -> to change

	* MODE #chan +t ok
	* MODE #chan -t ok

	* INVITE nickname #chan -ok
	* INVITE nickname #fake -ok
	* INVITE fake #chan -ok
	* MODE #1 +i -ok
	* MODE #1 -i -ok
	* JOIN #party jfhjkdshfdjskhfjdfsk
	* MODE #party +k 1234
	* MODE #party -k 1234
	* KICK #chan nick ->segfault

extra :
	* Welcome user
	


