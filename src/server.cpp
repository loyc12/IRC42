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

// 0================ OTHER FUNCTIONS ================0

void	Server::manageJoinCmd(std::string *args, User *user, int fd){
	//first check if the chan already exists on server
	std::map<std::string, Channel*>::iterator it = this->_chanContainer.find(args[1]);
	if (it != this->_chanContainer.end()){ //channel exists
		std::cout << "just join channel" << std::endl;
		//ft to check if there is a password
		it->second->joinChan(user, fd);
	}
	else { //channel does not exist
		std::cout << "add new channel to container" << std::endl;
		Channel *newChannel = new Channel(args[1]); //maybe need to deal with leaks
		this->_chanContainer.insert(std::pair<std::string, Channel*>(args[1], newChannel));
		newChannel->setNameChan(args[1]);
		newChannel->setAdmin(user->getNick());
		newChannel->joinChan(user, fd);
	}
}

int	Server::checkPassword(User* user, int fd, std::string pass)
{
	if (pass.compare(this->getPass()) != 0)
		return (badPassword(user, fd));
	else
		this->_welcomeFlag = 1;

	return (0);
}

int	Server::badPassword(User* user, int fd)
{
	debugPrint(RED, "0========= CONNECTION DENIED =========0");
	sendToClient(user, fd, ERR_NOSUCHCHANNEL, "Incorrect password");
	return (-1);
}

void	Server::storeNickname(User *user, int fd, std::string *args) {

	user->setNick(args[1]);
	if (this->_welcomeFlag == 1 && this->_isSet == 0)
	{
		this->welcomeMsg(user, fd);
		this->_isSet = 1;
	}
}

void	Server::welcomeMsg(User *user, int fd)
{
	sendToClient(user, fd, RPL_WELCOME, "Welcome to this IRC server");
}

// FT_I/O - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void	Server::sendToClient(User* user, int fd, std::string code, std::string input)
{
	std::ostringstream 	message;
	std::string 		result;

	message << ":irc.example.com " << code << " " << user->getNick() << " :" << input << "\r\n"; //	To change irc.example

	result = message.str();
	debugPrint(GREEN, result);
	if (send(fd, result.c_str(), result.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToClient() ");

	debugPrint(MAGENTA, "Code sended to Client\n");
}

int	Server::command(std::string	*args)
{
	int 		index = 0;
	std::string cmdArray[8] = {	"PASS", "NICK", "USER", "JOIN", "KICK", "INVITE", "TOPIC","MODE" };

	while (index < 8)
	{
//		If it's not a command, it's a normal message
		if (!cmdArray[index].compare(args[0]))
			break;
		index++;
	}
	return (index);
}

int	Server::readFromClient(User *user, int fd, std::string *message)
{
	char 		buff[BUFFSIZE];

	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);

//	Check byte have been reveived = if error, delete client. NOTE -> do we really want that ?
	if (byteReceived <= 0)
		return (deleteClient(fd, buff));
	else if (byteReceived)
	{
		std::string	*args = splitString(buff, " \r\n");

//		Ce serait cool un pointeur sur fonction ici a la place
		switch (command(args))
		{
//			Client is deleted if wrong password.
			case 0:
				if (this->checkPassword(user, fd, args[1]) < 0)
					return (deleteClient(fd, buff));
				std::cout << "\nSTEP 1" << std::endl;	// DEBUG
				break;
			case 1:
				this->storeNickname(user, fd, args);
				std::cout << "\nSTEP 2" << std::endl;	// DEBUG
				break;
			case 2:
				user->parseUserInfo(args);
				std::cout << "\nSTEP 3" << std::endl;	// DEBUG
				break;
			case 3:
				this->manageJoinCmd(args, user, fd);
				//channel->joinCmd(args);
				std::cout << "do stuff for join" << std::endl; //check JOIN #nameOfChannel password
				break;
			case 4:
				std::cout << "do stuff to be kick" << std::endl;
				break;
			case 5:
				std::cout << "do stuff for invite" << std::endl;
				break;
			case 6:
				std::cout << "do stuff to topic" << std::endl;
				break;
			case 7:
				std::cout << "do stuff to mode" << std::endl;
				break;
			default:
				this->_isMsg = 1;
		}
		message->assign(buff, 0, byteReceived);
		if (this->_isMsg == 1)
			sendToClient(user, fd, "302", *message);
		this->_isMsg = 0;
		debugPrint(GREEN, *message);

//		will need send according to what was done as a command (above)
		//ret = send(fd, message, message->length(), 0);
		// if (ret == 0)
		// {
		// 	std::cout << "HERE" << std::endl;
		// }
		// else if (ret < 0)
		// {
		// 	std::cout << "ERROR" << std::endl;
		// }
		bzero(buff, BUFFSIZE);
	}
	return (0);
}

// FT_CLIENT - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	DEBUG PRINT
void	Server::printClient(struct sockaddr_in *client_addr)
{
		std::cout << CYAN << CONNECTED << " > on socket(" << this->_newSocket <<
		") " << inet_ntoa(client_addr->sin_addr) <<
		":" << ntohs(client_addr->sin_port) << DEFCOL << "\n\n" << std::endl;
}

//	ENTRY POINT TO DISPATCH MESSAGE
void	Server::knownClient(int *clientFd)
{
	std::string	msg;
//	Find target client
	if (this->_it != this->_clients.end())
	{
//		This will need explanation from Val
		User* userPtr = this->_it->second;
		if (readFromClient(userPtr, *clientFd, &msg) < 0)
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
