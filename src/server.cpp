#include "IRC.hpp"

Server::Server(int port) : _port(port), _password("1234") 	{debugPrint(YELLOW, CONSTR_SERV); } //		TODO : take the actual PASSWORD
Server::~Server() 											{debugPrint(YELLOW, DEST_SERV); }

const int & Server::getPort			(void) const			{ return (this->_port);}
const std::string & Server::getPass	(void) const			{ return (this->_password);}



//	FT_CMD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// CHECKS PASSWORD AND SENDS AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, int fd, std::string *args)
{
//	If password is invalid
	if (args[1].compare(this->getPass()) != 0)
	{
		debugPrint(RED, DENIED); //													DEBUG
		std::string errMsg = "Invalid password";

		replyTo(REQUEST, user, fd, ERR_NOSUCHCHANNEL, errMsg);
		return (-1);
	}
	return (0);
}

int	Server::storeNickname(User *user, int fd, std::string *args)
{
	(void)fd;
	user->setNick(args[1]);
	return (0);
}

int	Server::storeUserInfo(User *user, int fd, std::string *args)
{
	//(void)fd;
	user->setUserInfo(args);

//	Welcomes as user if this is their first password check (aka first connection)
	if (!user->wasWelcomed)
		this->welcomeUser(user, fd);

	return (0);
}

int	Server::joinChannel(User *user, int fd, std::string *args)
{
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);

//	If the channel exists, try to join it (password managed in Channel class ?)
	if (it != this->_chanContainer.end())
	{
		debugPrint(MAGENTA, "\n > joinning a channel\n"); //									DEBUG

		it->second->joinChan(user, fd, JOIN, it->second->getChanName());
	}
//	Else channel does not exist
	else
	{
		debugPrint(MAGENTA, "\n > adding a new channel\n"); //									DEBUG

		Channel *newChannel = new Channel(args[1]); //											WARNING : may need to deal with leaks
		this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));

		newChannel->setChanName(args[1]);
		newChannel->setAdminName(user->getNick());
		//newChannel->joinChan(user, fd, JOIN, newChannel->getChanName());
		replyTo(CHAN, user, fd, JOIN, newChannel->getChanName());
	}

	return (0);
}

int	Server::kickUser(User *user, int fd, std::string *args)
{
	(void)user;
	(void)fd;
	(void)args;

	std::cout << "TODO : kick user out" << std::endl; //				DEBUG

	return (0);
}

int	Server::inviteUser(User *user, int fd, std::string *args)
{
	(void)user;
	(void)fd;
	(void)args;

	std::cout << "TODO : invite user in" << std::endl; //				DEBUG

	return (0);
}

int	Server::setChannelTopic(User *user, int fd, std::string *args)
{
	(void)user;
	(void)fd;
	(void)args;

	std::cout << "TODO : set channel topic" << std::endl; //			DEBUG

	return (0);
}

int	Server::setUserMode(User *user, int fd, std::string *args)
{
	(void)user;
	(void)fd;
	(void)args;

	std::cout << "TODO : set user mode" << std::endl; //				DEBUG

	return (0);
}

//	PROCESSES A MESSAGE THAT NEEDS TO BE BROADCASTED
int	Server::processMessage(User *user, int fd, std::string *args) //	TODO : implement me properly (sendToChannel() ?)
{
	(void)user;
	(void)fd;
	(void)args;

 			// for (int clientFd = 0; clientFd < FD_SETSIZE; ++clientFd)
			// {
        	// 	last_msg->assign(buff, 0, byteReceived);
        
        	// 	std::ostringstream debug; //											DEBUG
        	// 	debug << "INCOMING MSG FROM : (" << fd << ")\t| " << *last_msg; //		DEBUG
        	// 	debugPrint(GREEN, debug.str()); //										DEBUG
    
        	// 	replyTo(user, fd, RPL_REPLY, *last_msg); //		WARNING : RPL_REPLY, temp solution
			// }

//	std::cout << "TODO : proccess incoming message" << std::endl; //	DEBUG
	return (1);
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
int	Server::execCommand(User *user, int fd, std::string *args)
{
	int (Server::*commands[])(User*, int, std::string*) = {
		&Server::checkPassword,
		&Server::storeNickname,
		&Server::storeUserInfo,
		&Server::joinChannel,
		&Server::kickUser,
		&Server::inviteUser,
		&Server::setChannelTopic,
		&Server::setUserMode,
		&Server::processMessage
	};

	return (this->*commands[getCmdID(args[0])])(user, fd, args);
}

//	FT_I/O - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	SET THE HEADER AND SENDS A WELCOME MESSAGE ON CLIENT
void	Server::welcomeUser(User *user, int fd)
{
	replyTo(REQUEST, user, fd, RPL_WELCOME, WELCOME_HEADER);
	user->wasWelcomed = true;
	// TODO : You can :  PASS, NICK, JOIN
	// TODO:  You can't : MESSAGE, USER?, KICK, INVITE, TOPIC, MODE
	// TODO:  TRIGGER : Command ID
}


//	SENDS A SINGLE MESSAGE TO A SINGLE CLIENT //						TODO : create sendToChannel() and sendToAll()
void	Server::replyTo(int target, User* user, int fd, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

//	send structured fix template message to infobox of client (request) or to a chan of client (CHAN) (DONT TOUCH)
	if (target == REQUEST)
		message << ":" << user->getHostname() << " " << code << " " << user->getNick() << " :" << input << "\r\n";
	else if (target == CHAN)
		message << ":" << user->getNick() << "!" << user->getUsername() << "@" << user->getHostname() << " " << code << " " << input << "\r\n";

	result = message.str();
	std::ostringstream debug; //											DEBUG
	debug << "OUTGOING C_MSG TO : (" << fd << ")\t| " << result; //			DEBUG
	debugPrint(GREEN, debug.str()); //										DEBUG

	if (send(fd, result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at replyTo() ");
}

void	Server::readFromClient(User *user, int fd, std::string *last_msg)
{
	char 		buff[BUFFSIZE];

	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Handles what to do depending on the byte value (error, null or message)
	if (byteReceived < 0)
		throw std::invalid_argument(" > Error at rcv(): ");
	else if (byteReceived == 0)
	{
//		Deletes the client, loses its FD and clear and removes it from the baseFds
		deleteClient(fd, buff);
		close(fd);
		FD_CLR(fd, &(this->_baseFds));
	}
	else if (byteReceived > 0)
	{
		std::string	*args = splitString(buff, " \r\n");

//		Check if the message is a command, else manages it as a message
		if (execCommand(user, fd, args) == 1)
		{
			//processMessage(); < --- Déplacer là
        	last_msg->assign(buff, 0, byteReceived);
        
        	std::ostringstream debug; //											DEBUG
        	debug << "INCOMING MSG FROM : (" << fd << ")\t| " << *last_msg; //		DEBUG
        	debugPrint(GREEN, debug.str()); //										DEBUG
			//replyTo(CHAN, user, fd, NULL, *last_msg);
			//replyTo(REQUEST, user, )
        	replyTo(REQUEST, user, fd, RPL_REPLY, *last_msg); //						WARNING : RPL_REPLY, temp solution
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
		printClient(client_addr);
		User* user = new User(*client_addr);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		this->_it = this->_clients.find(this->_newSocket);
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

//	READS AN INCOMING MESSAGE FROM A ALREADY EXISTING CLIENT
void	Server::knownClient(int *clientFd)
{
	std::string	last_msg;
//	Finds target client
	if (this->_it != this->_clients.end())
	{
//		map<key, value>; second = value (value = User*)
		User* user = this->_it->second;
		readFromClient(user, *clientFd, &last_msg);
	}
}

//	DELETES A GIVEN CLIENT
void Server::deleteClient(int fd, char *buff)
{
//	Sets iterator to the client's Fd
	this->_it = this->_clients.find(fd);

//	Deletes all data from client's struct
	bzero(buff, BUFFSIZE);
	if (this->_it != this->_clients.end())
		delete this->_it->second;

//	Clears the fd from this client
	this->_clients.erase(fd);
	debugPrint(CYAN, DISCONNECTED);
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
	this->_serverAddr.sin_port = htons(this->getPort()); //		Ip Adress
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
						this->knownClient(&clientFd);
						std::cerr << "\n > HERE\n\n"; //			DEBUG
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
	this->_it = this->_clients.begin();
	std::map<int, User*>::iterator ite = this->_clients.end();

	while (this->_it != ite)
	{
		delete this->_it->second;
		this->_it++;
	}
	this->_clients.clear();
	close(this->_baseSocket);
	close(this->_newSocket);
}
