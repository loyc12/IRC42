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
Just for me to keep it in case:

void	Server::sendToChan(std::string message, std::vector<std::string> args)
{
	Channel *chan = findChannel(args[1]);

//	Sends a message to every channel member if it has at least 3 args (PRIVMSG + chan + message[0])
	if (chan != NULL) // && args.size() > 2)
	{
		std::cerr << "HERE" << std::endl; //									DEBUG
		for (int i = 0; i < chan->getMemberCnt(); i++)
			replyToChan(CHAN, chan, chan->getMember(i), "", message);
	}
}


void	Server::replyToChan(int target, Channel *chan, User* user, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

//	send structured fix template message to infobox of client (request) or to a chan of client (CHAN) (DONT TOUCH)
	if (target == REQUEST)
		message << ":" << user->getHostname() << " " << code << " " << user->getNick() << " :" << input << "\r\n";
	else if (target == CHAN)
		message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << code << " " << input << "\r\n";

	std::string listMembers;
	for (std::vector<User*>::iterator it = chan->_chanMembers.begin(); it != chan->_chanMembers.end(); it++)
	{
		listMembers += it->second.getNick() + " ";
	}


	message << ":" << " 331 " << user->getUsername() << " " << chan->getChanName() << " :" << chan->getTopic() << "\r\n";
	message << ":" << " 353 " << user->getUsername() << " = " << chan->getChanName() << " :" << listMembers << "\r\n";
	message << ":" << " 366 " << user->getUsername() << " " << chan->getChanName() << " :" << "End of NAMES list" << "\r\n";
	result = message.str();
	std::ostringstream debug; //												DEBUG
	debug << "OUTGOING C_MSG TO : (" << user->getFD() << ")\t| " << result; //	DEBUG
	debugPrint(GREEN, debug.str()); //											DEBUG

	if (send(user->getFD(), result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at replyTo() ");
}

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
	make sure you can't use the same nick as someone else (storeNickname)
	implmenet kickUser (message + remove from chan)
	implement setChannelTopic
	finish implementing mode (setChanMode ?) (make mode functions tage +/- as args)
	remove user from all channels in QUIT
	rework invite message (invite #1 to)
