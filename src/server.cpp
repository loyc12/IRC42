#include "IRC.hpp"

# define CONSTR_PARAM "0========= PARAM-CONSTR(SERVER) ======0"
# define CONSTR_COPY "0======== COPY-CONSTR(SERVER) ========0"
# define CONSTR_ASSIGN "0======== ASSIGN-CONSTR(SERVER) ========0"
# define DESTRUCT "0======== DESTRUCT-(SERVER) ========0"
# define LAUNCH "\n0========== SERVER LAUNCHED ==========0"
# define DISCONNECTED "\n0========= CONNECTION DENIED =========0"

// 0================ BASE FUNCTIONS ================0

void	Server::debugPrint(std::string color, std::string message)		{std::cout << color << message << DEFCOL;}
Server::Server(int port) : _port(port), _password("1234") 				{debugPrint(YELLOW, CONSTR_PARAM); }
Server::~Server() 														{debugPrint(YELLOW, DESTRUCT); }

const int & Server::getPort(void) const					{ return (this->_port);}
const std::string & Server::getPass(void) const			{ return (this->_password);}

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

int	Server::readFromClient(User *user, int fd, std::string *message)
{
	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);
	if (byteReceived <= 0)
		return (deleteClient(fd, buff));
	else if (byteReceived)
	{
		std::string	*args = splitString(buff, " \r\n");

		/*--switch case implementation--*/
		std::string cmdArray[8] = {
			"PASS",
			"NICK",
			"USER",
			"JOIN",
			"KICK",
			"INVITE",
			"TOPIC",
			"MODE"
		};
		int index = 0;
		while (index < 8) {
			if (!cmdArray[index].compare(args[0]))
				break;
			index++;
		}
		switch (index) {
			case 0:
				if (this->checkPassword(user, fd, args[1]) < 0)
					return (deleteClient(fd, buff));
				break;
			case 1:
				this->storeNickname(user, fd, args);
				break;
			case 2:
				user->parseUserInfo(args);
				std::cout << user->getUsername() << " " << user->getMode() << std::endl; //
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
//			default:
//				std::cout << "Command does not exist" << std::endl; //msg to be send to client though..
		}
		message->assign(buff, 0, byteReceived);
		std::cout << *message;

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

void	Server::sendToClient(User *user, int fd, std::string msg)
{
	(void)user;
	std::cout << "MSG SEND TO CL:\t" << msg << std::endl;
	if (send(fd, msg.c_str(), msg.size(), 0) < 0)
		throw std::invalid_argument(" > Error at sendToClient() ");
}

void	Server::responseToClient(User* user, int fd, std::string code, std::string message)
{
	std::string response = ": " + code + " " + user->getNick() + " :" + message + "\r\n";
	sendToClient(user, fd, response);
	std::cout << "Code sended to Client" << std::endl;
	close(fd);
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
	std::cout << std::endl << RED << "0========= CONNECTION DENIED =========0" << DEFCOL << std::endl;
	std::string errMsg = "Incorrect password";
	responseToClient(user, fd, ERR_NOSUCHCHANNEL, errMsg);
	return (-1);
}

void	Server::storeNickname(User *user, int fd, std::string *args) {

	user->setNick(args[1]);
	std::cout << "nickname: " << user->getNick() << std::endl;
	if (this->_welcomeFlag == 1 && this->_isSet == 0){
		this->welcomeMsg(user, fd);
		this->_isSet = 1;
		std::cout << "Pass && nickname done" << std::endl;
	}
}

void	Server::welcomeMsg(User *user, int fd) {

	std::ostringstream welcome;
	welcome << ":" << RPL_WELCOME << user->getNick() << " :Welcome to this IRC server" << "\r\n";

	sendToClient(user, fd, welcome.str());
}

void	Server::knownClient(std::map<int, User*>::iterator it, int *i){

	std::string	msg;

	if (it != this->_clients.end()){

		User* userPtr = it->second;
		if (readFromClient(userPtr, *i, &msg) < 0) {
			close(*i);
			FD_CLR(*i, &this->_baseFds);
		}
	}
}


void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len, std::map<int, User*>::iterator *it)
{

	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *) &*client_addr, &*client_len);
	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > Error at accept(): ");
	else
	{

		std::cout << CYAN << "\n0========== CLIENT CONNECTED =========0\n" << " > on socket("
		<< this->_newSocket << ") " << inet_ntoa(client_addr->sin_addr)
		<< ":" << ntohs(client_addr->sin_port) << DEFCOL << "\n\n" << std::endl;

		User* user = new User(*client_addr);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		*it = this->_clients.find(this->_newSocket);
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

int		Server::deleteClient(int fd, char *buff)
{
	std::map<int, User*>::iterator it = this->_clients.find(fd);

	bzero(buff, BUFFSIZE);
	if (it != this->_clients.end())
			delete it->second;

	this->_clients.erase(fd);
	debugPrint(CYAN, DISCONNECTED);
	return (-1);
}

//	BASE SOCKET OF SERVER (ENTRY POINT)
void	Server::initServ()
{
	//struct sockaddr_in	server_addr;
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
	std::map<int, User*>::iterator	it = this->_clients.begin();
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

//	SERVER ENTRY POINT
void	Server::start(void)
{
	std::map<int, User*>::iterator	it;
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
		else if (this->_socketCount) { for (int i = 0; i < FD_SETSIZE; ++i) { if (FD_ISSET(i, &this->_targetFds))
		{
//			A Valid FD have been Found.
			if (i == this->_baseSocket)
				this->newClient(&client_addr, &client_len, &it);
			else
				this->knownClient(it, &i);
		}}}
	}
	clearServ();
}
