#include "Server.hpp"

Server::Server(int port) : _port(port), _password("1234") 	{debugPrint(YELLOW, CONSTR_SERV); } //		TODO : take the actual PASSWORD
Server::~Server() 											{debugPrint(YELLOW, DEST_SERV); }

const int & Server::getPort			(void) const			{ return (this->_port);}
const std::string & Server::getPass	(void) const			{ return (this->_password);}



//	FT_CMD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, std::vector<std::string> args)
{
//	If password is invalid
	if (args[1].compare(this->getPass()) != 0)
	{
		debugPrint(RED, DENIED); //										DEBUG
		std::string errMsg = "Invalid password";

		replyTo(REQUEST, user, ERR_NOSUCHCHANNEL, errMsg);
		return (-1);
	}
	return (0);
}

int	Server::storeNickname(User *user, std::vector<std::string> args)
{
	user->setNick(args[1]);
	return (0);
}

int	Server::storeUserInfo(User *user, std::vector<std::string> args)
{
	user->setUserInfo(args);

//	Welcomes as user if this is their first password check (aka first connection)
	if (!user->wasWelcomed)
		this->welcomeUser(user);

	return (0);
}

int	Server::kickUser(User *user, std::vector<std::string> args) // , Channel *chan)
{
	(void)user;
	(void)args;

	std::cout << "TODO : kick user out" << std::endl; //				DEBUG

	return (0);
}

int	Server::inviteUser(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : invite user in" << std::endl; //				DEBUG

	return (0);
}

int	Server::setChannelTopic(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : set channel topic" << std::endl; //			DEBUG

	return (0);
}

int	Server::setUserMode(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	std::cout << "TODO : set user mode" << std::endl; //				DEBUG

	return (0);
}

//	TELLS readFromClient that this is a message
int	Server::processMessage(User *user, std::vector<std::string> args)
{
	(void)user;
	(void)args;

	return (-1);
}

//	GETS THE SPECIFIC ID OF A USER COMMAND
int Server::getCmdID(std::string cmd)
{
	std::string cmds[8] = {	"PASS", "NICK", "USER", "JOIN", "KICK", "INVITE", "TOPIC","MODE" };

	int id = 0;
	while (id < 8 && cmd.compare(cmds[id]))
		id++;

	return (id);
}

//	PICKS A COMMAND TO EXECUTE BASED ON THE ARGS
int	Server::execCommand(User *user, std::vector<std::string> args)
{
	int (Server::*commands[])(User*, std::vector<std::string>) = {
		&Server::checkPassword,
		&Server::storeNickname,
		&Server::storeUserInfo,
		&Server::cmdJoin,
		&Server::kickUser,
		&Server::inviteUser,
		&Server::setChannelTopic,
		&Server::setUserMode,
		&Server::processMessage
	};

	return (this->*commands[getCmdID(args[0])])(user, args);
}

//	FT_I/O - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user)
{
	replyTo(REQUEST, user, RPL_WELCOME, WELCOME_HEADER);
	user->wasWelcomed = true;
}

bool	Server::isUserInChan(User *user, Channel *chan)
{
	if (chan->hasMember(user))
	{
		replyTo(REQUEST, user, ERR_ALREADYREGISTRED, "Already registered");
	 	return (true);
	}
	return (false);
}

bool	Server::checkInvitePerm(User *user, Channel *chan)
{
//	Check if the mode of the chan is in Invitation only
	if (chan->getInviteOnly())
	{
		replyTo(REQUEST, user, ERR_INVITEONLYCHAN, "The channel is invite only");
		return (false);
	}
	return (true);
}

bool	Server::checkPass(User *user, Channel *chan, std::string pass)
{
//	Check password of channel (if they are one)
	if (chan->getPass().length() != 0)
	{
//		Check password provided, return error if no or bad password
		if (pass.length() == 0)
		{
			replyTo(REQUEST, user, ERR_NEEDMOREPARAMS, "Not password provided");
			return (false);
		}
		else if (pass.compare(chan->getPass()) != 0)
		{
			replyTo(REQUEST, user, ERR_PASSWDMISMATCH, "Password incorrect");
			return (false);
		}
	}
	return (true);
}

bool	Server::checkMaxMbr(User *user, Channel *chan)
{
//	Check if we can add a member to the channel list
	if (chan->getMaxMbrCnt() > 0 && chan->getMemberCnt() >= chan->getMaxMbrCnt())
	{
		replyTo(REQUEST, user, ERR_CHANNELISFULL, "Cannot join channel");
		return (false);
	}
	return (true);
}

void	Server::knownChannel(User *user, Channel *chan, std::vector<std::string> args)
{
//	Check all conditions in mode if we can add the member to this channel
	if (!isUserInChan(user, chan) && checkInvitePerm(user, chan) \
		&& checkPass(user, chan, args[2]) && checkMaxMbr(user, chan)) //		NOTE : these send their own error messages
	{
//		the client can enter the channel
		debugPrint(MAGENTA, "\n > joinning a channel\n"); // DEBUG
		chan->addMember(user);
//		replyTo(REQUEST, user, ??, chan->getChanName()); // send info message to request
		replyTo(CHAN, user, JOIN, chan->getChanName()); 	// send code to trigger the chan invite
//		sendToChan(*lastMsg, args); // send code (alert ou prv msg) to all membres of chan
	}
}

void	Server::newChannel(User *user, std::vector<std::string> args)
{
//		check if only 1 arg (chanName) provided, else error
// 		if (args[2].length() != 0) //															NOTE : can't check for missing arg like this (?)
// 		{
// 			If more information is added, we assume the client wanted to join a channel, so we block the creation
// 			replyTo(REQUEST, user, "403", "No Such Channel");//ERR_NOSUCHCHANNEL
// 			return (0);
// 		}
		//	else, create the channel
		debugPrint(MAGENTA, "\n > adding a new channel\n"); //									DEBUG
		Channel *newChannel = new Channel(args[1]); //											WARNING : may need to deal with leaks
		this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));
		newChannel->setChanName(args[1]);

		newChannel->addMember(user);
		newChannel->setAdminName(user->getNick());

		replyTo(CHAN, user, JOIN, newChannel->getChanName());
}

int	Server::cmdJoin(User *user, std::vector<std::string> args) //								DEBUG NOTE THINGIES : this doesn't really work rn. seems like it doesn't join the same channel despite the name
{
//	If join have no channel name, it return "#". We use "#" to return an error code.
	if (args[1].compare("#") == 0)
		replyTo(REQUEST, user, ERR_NEEDMOREPARAMS, "Need more params");
	else
	{
		std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

	//	if the channel exists, try to join it. else create it
		if (it != this->_chanContainer.end())
			knownChannel(user, it->second, args);
		else
			newChannel(user, args);
	}
	return (0);
}

//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT
void	Server::replyTo(int target, User* user, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

//	send structured fix template message to infobox of client (request) or to a chan of client (CHAN) (DONT TOUCH)
	if (target == REQUEST)
		message << ":" << user->getHostname() << " " << code << " " << user->getNick() << " :" << input << "\r\n";
	else if (target == CHAN)
		message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << code << " " << input << "\r\n";

	result = message.str();
	std::ostringstream debug; //												DEBUG
	debug << "OUTGOING C_MSG TO : (" << user->getFD() << ")\t| " << result; //	DEBUG
	debugPrint(GREEN, debug.str()); //											DEBUG

	if (send(user->getFD(), result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at replyTo() ");
}


Channel	*Server::findChannel(std::string str)
{
	str.erase(0, 1);
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(str);

	//if (it != this->_chanContainer.end())
	{
		std::cerr << "HERE" << std::endl; //									DEBUG
		return (it->second);
	}
	//else
		return (NULL);
}

//	SENDS A SINGLE MESSAGE TO ALL MEMBERS OF A CHANNEL
void	Server::sendToChan(std::string message, std::vector<std::string> args)
{
	Channel *chan = findChannel(args[1]);

//	Sends a message to every channel member if it has at least 3 args (PRIVMSG + chan + message[0])
	if (chan != NULL) // && args.size() > 2)
	{
		std::cerr << "HERE" << std::endl; //									DEBUG
		for (int i = 0; i < chan->getMemberCnt(); i++)
			replyTo(CHAN, chan->getMember(i), "", message);
	}
}

void	Server::readFromClient(User *user, int fd, std::string *lastMsg)
{
	char 		buff[BUFFSIZE];

	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Handles what to do depending on the byte value (error, null or message)
	if (byteReceived < 0)
		throw std::invalid_argument(" > Error at rcv(): ");
	else if (byteReceived == 0)
	{
//		Deletes the client, loses its FD and removes it from the baseFds
		deleteClient(fd, buff);								//TODO : EXPLICATION NOTE : this makes the server clear the client data
	}
	else if (byteReceived > 0)
	{
        lastMsg->assign(buff, 0, byteReceived);

		std::vector<std::string> args = splitString(buff, " \r\n");

		if (execCommand(user, args) == -1)
		{
//			//Send message to all in the chan


        	std::ostringstream debug; //											DEBUG
        	debug << "INCOMING MSG FROM : (" << fd << ")\t| " << *lastMsg; //		DEBUG
        	debugPrint(GREEN, debug.str()); //										DEBUG

			//replyTo(CHAN, user, NULL, *lastMsg);
			//replyTo(REQUEST, user, )

			std::cerr << args[0] << std::endl; //									DEBUG

			if (args[0].compare("PRIVMSG") != 0)
        		replyTo(CHAN, user, "", *lastMsg);
			else
				sendToChan(*lastMsg, args);
		}
	}
	bzero(buff, BUFFSIZE);
}

// FT_CLIENT - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	DEBUG PRINT
void	Server::printClient(struct sockaddr_in *client_addr)
{
	std::cout << CYAN << CONNECTED << " > on socket(" <<
	this->_newSocket << ") " << inet_ntoa(client_addr->sin_addr) <<
	":" << ntohs(client_addr->sin_port) << DEFCOL<< std::endl << std::endl;
}

//	PARSE THE DATA FROM A NEW CLIENT'S FIRST MESSAGES
void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len)
{
//	Generates a new socket connection to adds to baseSocket
	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *)&*client_addr, &*client_len);

	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > Error at accept(): ");
	else
	{
//		Creates a new user for this new Socket & stores User container inside the Server
		printClient(client_addr); //										DEBUG
		User* user = new User(*client_addr);
		user->setFD(this->_newSocket);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

//	READS AN INCOMING MESSAGE FROM A ALREADY EXISTING CLIENT
void	Server::knownClient(int fd)
{
	std::string	lastMsg;
	std::map<int, User*>::iterator it = this->_clients.find(fd);

//	Finds target client
	if (it != this->_clients.end())
	{
		User *user = it->second;
		readFromClient(user, fd, &lastMsg);
	}
}

//	DELETES A GIVEN CLIENT
void Server::deleteClient(int fd, char *buff)
{
//	Sets iterator to the client's Fd
	std::map<int, User*>::iterator it = this->_clients.find(fd);

//	Deletes all data from client's struct
	bzero(buff, BUFFSIZE);
	if (it != this->_clients.end())
		delete it->second;

//	Clears the fd from this client
	this->_clients.erase(fd);
	debugPrint(CYAN, DISCONNECTED);

//	Removes fd from _baseFds
	close(fd);
	FD_CLR(fd, &(this->_baseFds));
}


//	FT_SERVER - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	INITS THE SERVER PROCESSES
void	Server::init()
{
//	Sets BaseSocket (Entry point)
	this->_baseSocket 	= socket(AF_INET, SOCK_STREAM, 0);
	if (this->_baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");

//	Sets Options for BaseSocket
	const int reuse = 1;
	if (setsockopt(this->_baseSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		throw std::invalid_argument(" > Error at setsocketopt(): ");

//	Sets Server Adress
	bzero(&this->_serverAddr, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET; //					bind call
	this->_serverAddr.sin_port = htons(this->getPort()); //		IP Adress
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY; //			localhost

//	Connects server's port
	if (bind(this->_baseSocket, (struct sockaddr *) &this->_serverAddr, sizeof(this->_serverAddr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

//	Sets to receive all conections
	listen(this->_baseSocket, SOMAXCONN);
	FD_ZERO(&this->_baseFds);
	FD_SET(this->_baseSocket, &this->_baseFds);

	debugPrint(GREEN, LAUNCH); //								DEBUG
}

//	LAUNCHES THE SERVER AND ITS PROCESSES
void	Server::start(void)
{
	struct sockaddr_in				client_addr;
	socklen_t 						client_len = sizeof(client_addr);

	this->init();

	while (!shutServ)
	{
		this->_targetFds = this->_baseFds;
		this->_socketCount = select(FD_SETSIZE, &this->_targetFds, nullptr, nullptr, nullptr);

//		Checks validity of received socket fd (if error, break / throw)
		if (this->_socketCount == -1)
		{
//			If CTRL-C at select, treat as not an error.
			if (errno == EINTR)
				break ;
			throw std::invalid_argument(" > Error at select(): ");
		}
		else if (this->_socketCount > 0)
		{
			for (int clientFd = 0; clientFd < FD_SETSIZE; ++clientFd)
			{
//				Check if the bit in the Fd is setted.
				if (FD_ISSET(clientFd, &this->_targetFds))
				{
//					Manages the client either as a new or old one, depending on its FD
					if (clientFd == this->_baseSocket)
						this->newClient(&client_addr, &client_len);
					else
					{
						this->knownClient(clientFd);
//						std::cerr << "\n > HERE\n\n"; //			DEBUG
					}
				}
			}
		}
	}
	this->clear();
}

//	CLOSES THE SERVER SAFELY
void	Server::clear(void)
{
	debugPrint(MAGENTA, CLOSING); //								DEBUG
	std::map<int, User*>::iterator it = this->_clients.begin();
	std::map<int, User*>::iterator ite = this->_clients.end();

	while (it != ite)
	{
		delete it->second;
		it++;
	}
	this->_clients.clear();
	close(this->_baseSocket);
	close(this->_newSocket);
}
