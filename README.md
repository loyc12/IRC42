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

## Problem:





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

