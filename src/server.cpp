#include "IRC.hpp"

Server::Server(int port) : _port(port), _password("1234") 			{debugPrint(YELLOW, CONSTR_SERV); }
Server::~Server() 													{debugPrint(YELLOW, DEST_SERV); }
const int & Server::getPort(void) const								{ return (this->_port);}
const std::string & Server::getPass(void) const						{ return (this->_password);}

std::ostream &operator<< (std::ostream &out, const Server &rhs)
{
	out << "IRC Server port : " << rhs.getPort();
	return (out);
}


//	FT_CMD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -




int	Server::storeUserInfo(User *user, int fd, std::string *args)
{
	(void)fd;
	user->setUserInfo(args);

	return (0);
}

int	Server::joinChannel(User *user, int fd, std::string *args) //							TODO : rework me?
{
	//first check if the chan already exists on server
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	if (it != this->_chanContainer.end()){ //channel exists
		std::cout << "just join channel" << std::endl;
		//ft to check if there is a password
		it->second->joinChan(user, fd);
	}
	else { //channel does not exist
		std::cout << "add new channel to container" << std::endl; //								DEBUG
		Channel *newChannel = new Channel(args[1]); //												WARNING : may need to deal with leaks
		this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));
		newChannel->setNameChan(args[1]);
		newChannel->setAdmin(user->getNick());
		newChannel->joinChan(user, fd);
	}

	return (0);
}

//	PROCESSES A MESSAGE THAT NEEDS TO BE BROADCASTED
int	Server::processMessage(User *user, int fd, std::string *args) //	TODO : implement me properly (sendToChannel() ?)
{
	(void)user;
	(void)fd;
	(void)args;

//	std::cout << "TODO : proccess incoming message" << std::endl; //	DEBUG
	return (1);
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

//	FT_ID - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	EXIT POINT TO SET CLIENT ID
int	Server::storeNickname(User *user, int fd, std::string *args)
{
	user->setNick(args[1]);
	if (this->_welcomeFlag == 1 && this->_isSet == 0)
	{
		this->welcomeMsg(user, fd);
		this->_isSet = 1;
	}
	return (0);
}

//	SET THE HEADER AND WILL SEND A WELCOME MESSAGE ON CLIENT 
void	Server::welcomeMsg(User *user, int fd)//					WARNING : Header ne set pas, mais ceci n'est pas une priorité
{
	sendToClient(user, fd, RPL_WELCOME, "Welcome to this IRC server");
}

// ENTRE POINT : CHECK PASSWORD AND SEND AN ERROR CODE TO CLIENT IF WRONG
int	Server::checkPassword(User *user, int fd, std::string *args)
{
	if (args[1].compare(this->getPass()) == 0)
	{
		this->_welcomeFlag = 1;
		return (0);
	}
	debugPrint(RED, DENIED); //										DEBUG
	std::string errMsg = "Invalid password";
	sendToClient(user, fd, ERR_NOSUCHCHANNEL, errMsg);
	return (-1);
}

//	FT_CMD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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


//	TODO : make a sendToChannel that loops on all channel's clients
void	Server::sendToClient(User* user, int fd, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

	message << ":irc.example.com " << code << " " << user->getNick() << " :" << input << "\r\n";
	result = message.str();

	std::ostringstream debug; //										DEBUG
	debug << "OUTGOING C_MSG TO : (" << fd << ")\t| " << result; //		DEBUG
	debugPrint(GREEN, debug.str()); //									DEBUG

	if (send(fd, result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToClient() ");
}

int	Server::readFromClient(User *user, int fd, std::string *last_msg)
{
	char 		buff[BUFFSIZE];

	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Check byte have been received = if error, delete client.				WARNING : do we really want that ?
	if (byteReceived <= 0)
		return (deleteClient(fd, buff));
	else if (byteReceived)
	{
		std::string	*args = splitString(buff, " \r\n");

//		if is not a command : manage the message (temp)
		if (execCommand(user, fd, args) == 1)
		{
			last_msg->assign(buff, 0, byteReceived);

			std::ostringstream debug; //											DEBUG
			debug << "INCOMING MSG FROM : (" << fd << ")\t| " << *last_msg; //		DEBUG
			debugPrint(GREEN, debug.str()); //										DEBUG

			sendToClient(user, fd, RPL_REPLY, *last_msg); //		WARNING : RPL_REPLY, temp solution
		}
		bzero(buff, BUFFSIZE);
	}
	return (0);
}

// FT_CLIENT - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	DEBUG PRINT
void	Server::printClient(struct sockaddr_in *client_addr)
{
		std::cout << CYAN << CONNECTED << " > on socket(" <<
		this->_newSocket << ") " << inet_ntoa(client_addr->sin_addr) <<
		":" << ntohs(client_addr->sin_port) << DEFCOL<< std::endl << std::endl;
}

//	ENTRY POINT TO DISPATCH MESSAGE
void	Server::knownClient(int *clientFd)
{
	std::string	last_msg;
//	Find target client
	if (this->_it != this->_clients.end())
	{
//		This will need explanation from Val
		User* userPtr = this->_it->second;
		if (readFromClient(userPtr, *clientFd, &last_msg) < 0)
		{
//			Close the client Fd and clear it in baseFds
			close(*clientFd);
			FD_CLR(*clientFd, &this->_baseFds);
		}
	}
}

//	DELETE TARGET CLIENT EXIT POINT
int Server::deleteClient(int fd, char *buff)
{
//	Set iterator to the client's Fd
	this->_it = this->_clients.find(fd);

//	Iterate to delete all data from client's struct
	bzero(buff, BUFFSIZE);
	if (this->_it != this->_clients.end())
			delete this->_it->second;

//	Clear the fd from this client
	this->_clients.erase(fd);
	debugPrint(CYAN, DISCONNECTED);

//	-1 will trigger to close and clear fd from base socket in known client
	return (-1);
}

// NEW CLIENT ENTRY POINT
void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len)
{
//	Generate new socket connection to add to baseSocket
	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *)&*client_addr, &*client_len);
	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > Error at accept(): ");
	else
	{
//		Create a new container user for this new Socket, insert User container inside Server's data
		printClient(client_addr);
		User* user = new User(*client_addr);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		this->_it = this->_clients.find(this->_newSocket);
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

//	FT_SERVER - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	BASE SOCKET OF SERVER (ENTRY POINT)
void	Server::initServ()
{
//	Set BaseSocket (Entry point)
	this->_baseSocket 	= socket(AF_INET, SOCK_STREAM, 0);
	if (this->_baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");

//	Set Options for BaseSocket
	const int reuse = 1;
	if (setsockopt(this->_baseSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		throw std::invalid_argument(" > Error at setsocketopt(): ");

//	Set Server Adress
	bzero(&this->_serverAddr, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;					//	bind call
	this->_serverAddr.sin_port = htons(this->getPort());	//	Ip Adress
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY; 		//	localhost

//	Connect server's port
	if (bind(this->_baseSocket, (struct sockaddr *) &this->_serverAddr, sizeof(this->_serverAddr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

//	Set to receive all conections
	listen(this->_baseSocket, SOMAXCONN);
	FD_ZERO(&this->_baseFds);
	FD_SET(this->_baseSocket, &this->_baseFds);
}

//	SERVER EXIT POINT
void	Server::clearServ(void)
{
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

//	SERVER ENTRY POINT
void	Server::start(void)
{
	struct sockaddr_in				client_addr;
	socklen_t 						client_len = sizeof(client_addr);

	this->initServ();
	debugPrint(GREEN, LAUNCH);
	while (!shutServ)
	{
		this->_targetFds = this->_baseFds;
		this->_socketCount = select(FD_SETSIZE, &this->_targetFds, nullptr, nullptr, nullptr);
		if (this->_socketCount == -1)
		{
//			If CTRL-C at select, it's not an error.
			if (EINTR)
				break ;

			throw std::invalid_argument(" > Error at select(): ");
		}
		else if (this->_socketCount)
		{
//			If socket is connected
			for (int clientFd = 0; clientFd < FD_SETSIZE; ++clientFd)
			{
//				Check if the bit in the Fd is setted.
				if (FD_ISSET(clientFd, &this->_targetFds))
				{
//					A Valid FD have been Found.
					if (clientFd == this->_baseSocket)
						this->newClient(&client_addr, &client_len);
					else
						this->knownClient(&clientFd);
				}
			}
		}
	}
	clearServ();
}
