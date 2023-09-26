#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
Server::Server() : _port(6667), _password("1234"), _baseSocket(0), _newSocket(0), _nameServer("ircserv"){
	std::cout << YELLOW << ": Called default constructor (SERVER) " << DEFCOL;
}

//public
Server::Server(int port) : _port(port), _password("1234") {
	std::cout << YELLOW << ": Called parameterized constructor (SERVER) " << DEFCOL;
}
Server::Server(const Server &other)
{
	std::cout << YELLOW << ": Called copy constructor (SERVER) " << DEFCOL;
	this->_port = other.getPort();
}
Server &Server::operator= (const Server &other)
{
	std::cout << YELLOW << ": Called assignment operator (SERVER) " << DEFCOL;
	this->_port = other.getPort();
	return *this;
}

Server::~Server()
{
	std::cout << YELLOW << ": Called destructor (SERVER) " << DEFCOL;
	//TODO : FREE
}
std::ostream &operator<< (std::ostream &out, const Server &rhs)
{
	out << "IRC Server port : " << rhs.getPort();
	return (out);
}

// 0================ GETTERS / SETTERS ================0

const int & Server::getPort(void) const { return (this->_port);}
const std::string & Server::getPass(void) const { return (this->_password);}
const std::string & Server::getNameServer(void) const { return (this->_nameServer);}

// 0================ OTHER FUNCTIONS ================0

//pass -> already verified here
int Server::checkPassword(std::string pass, int fd, User* user)
{
	//starting cleanup
	(void)user;
	int ret;

	if (pass.compare(this->getPass()) != 0)//check si c'est le bon mot de passe
	{
		/*
		std::ostringstream ss;
		ss << ":" << this->getNameServer() << " 001" << user->getNick() << " :Welcome to this IRC server, " << user->getNick() << "!" << user->getName() << "@" << this->getNameServer() << "\r\n";
		//TODO: enum list for numeric code, 001
		std::string welcome = ss.str();

		ret = send(fd, welcome.c_str(), welcome.size(), 0);
		*/


		//task = select et/ou voir le code quand un client deconnecte, probablement cela qu'il faut faire TODO : closing its FD and telling them to fuck off
		std::cout << std::endl << RED << "0========= CONNECTION DENIED =========0" << DEFCOL << std::endl;
		//std::string errMsg = "Incorrect password";
		//std::string response = ":" + this->getNameServer() + " 464" + user->getNick() + "\n";
		std::ostringstream errMsg;
		std::cout << this->getNameServer() << std::endl;
		errMsg << ":" << user->getHostname() << " 464 :Incorrect password\n";
		//TODO create a enum or list of status code/numeric codes
		std::string reply = errMsg.str();

    	// Send the error message to the LimeChat client
		send(fd, reply.c_str(), reply.size(), 0);
		close(fd);//a enlever eventuellement, autre solution.
		std::map<int, User*>::iterator it = this->_clients.find(fd);

		if (it != this->_clients.end())
			delete it->second;
		this->_clients.erase(fd);
		std::cout << std::endl << std::endl;
		return (-1);
	}
	else //	 ----------------------------------------------------------------------------------------------------------- WELCOME MESSAGE HERE
	{
		//syntax below on how to send msg to Limechat
		/*oss << ":" << m_hostname << " 001 " << m_userDB[fd].m_nickname << " :Welcome to the IRCServ, " << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username << "@" << m_hostname << "\r\n";*/
		std::ostringstream ss;
		ss << ":" << this->getNameServer() << " 001" << user->getNick() << " :Welcome to this IRC server, " << user->getNick() << "!" << user->getName() << "@" << this->getNameServer() << "\r\n";
		//TODO: enum list for numeric code, 001
		std::string welcome = ss.str();

		ret = send(fd, welcome.c_str(), welcome.size(), 0);
		//Code de gestion d'erreur
		if (ret == 0)
		{
			std::cout << "DISCONNECT ?" << std::endl;
		}
		else if (ret < 0)
		{
			std::cout << "ERROR" << std::endl;
		}
		else if (ret > 0)
		{
			std::cout << "YEAH" << std::endl;
		}
	}
	return (0);
}

int	Server::disconnectClient(char *buff, int fd)
{
	/*
	1. clear buffer
	2. delete client from container with std::map
	*/
	bzero(buff, BUFFSIZE);
	std::cout << std::endl << CYAN << "0======== CLIENT DISCONNECTED ========0" << DEFCOL << std::endl << std::endl;
	std::map<int, User*>::iterator it = this->_clients.find(fd);
	if (it != this->_clients.end())
			delete it->second;
	this->_clients.erase(fd);
	std::cout << std::endl << std::endl;
	return (-1);
}

//TODO Tuesday morning problem to finish
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

/**
 * @brief to read what the client sent and int for success or fail.
 *
 * @param fd
 * @param message
 * @param user
 * @return int
 */
int	Server::readFromClient(int fd, std::string *message, User *user)
{
	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);
	if (byteReceived <= 0)
		return (disconnectClient(buff, fd));
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
				if (this->checkPassword(args[1], fd, user) < 0)
					return (disconnectClient(buff, fd));
				break;
			case 1:
				user->setNick(args[1]);
				std::cout << "nickname: " << user->getNick() << std::endl;
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
			default:
				std::cout << "Command does not exist" << std::endl; //msg to be send to client though..
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

void	Server::knownClient(std::map<int, User*>::iterator it, int *i){

	std::string	message;

	if (it != this->_clients.end()){

		User* userPtr = it->second;
		if (readFromClient(*i, &message, userPtr) < 0) {
			close(*i);
			FD_CLR(*i, &this->_baseFds);
		}
	}
}

void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len, std::map<int, User*>::iterator *it)
{
	/*
	Variables : Pointer to an new Objet User
	1. accept();	set the new socket, if accepted the new client is connected.
	2. new User();	create new object user. At this point, no data as been received from Client except the connection.
//	[X] After new User, Implement a condition to block a Client if password (first output) is not OK, delete user and return.
	*/
	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *) &*client_addr, &*client_len);
	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > Error at accept(): ");
	else
	{
		std::cout << CYAN << "\n0========== CLIENT CONNECTED =========0\n" << " > on socket : "
		<< this->_newSocket << " " << inet_ntoa(client_addr->sin_addr)
		<< ":" << ntohs(client_addr->sin_port) << DEFCOL << "\n\n" << std::endl;

		User* user = new User(*client_addr);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		*it = this->_clients.find(this->_newSocket);
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

void	Server::init()
{
	/*
	Variables : Structure for server info. const int setted later bf edge case.
	1. socket();		Init baseSocket
	2. setsockopt();	Setting options with a reuse option bool target (to use if available)
	3. prepare for bind call
		bzero();		Cleaning servers adress and setting new data information.
		3.1				AF_INET = bind call
		3.2				htons	= conversion to network byte order (Ip adress)
		3.3				INADDR_ANY = localhost
	4. bind();			connect the server's port
	5. listen();		sets up baseSocket to receive all connections
	*/

	struct sockaddr_in	server_addr;

	this->_baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");

	const int reuse = 1;
	if (setsockopt(this->_baseSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		throw std::invalid_argument(" > Error at setsocketopt(): ");

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->getPort());
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

	listen(this->_baseSocket, SOMAXCONN);
}


void	Server::start(void)
{
	/*
	Variables : Structure for clients info, a buffer size for clients and a Container for User.
	1. init();	Setup our server (binding and socket)
	2. FD_();	Prepares fds for select
	3. while();
		3.1.	select(); Return qt of active sockets. Return error if (-1)
		3.2.	FD_ISSET(); Target Fd is compared will all active sockets.
				IF target = base socket = its a new client, otherwise it already in the system.
	4. If Shutserv = close sockets.
//	[X] Checking for active and non-active socket (wdm?) at select();
!	[ ] Checking leaks (Malloc in User)... Checking at Deleted and at not deleted.
!	[ ] Problem at knownClient but we didnt write the reason so ?
*/

	struct sockaddr_in	client_addr;
	socklen_t 			client_len = sizeof(client_addr);
	std::map<int, User*>::iterator it;

	this->init();
	FD_ZERO(&this->_baseFds);
	FD_SET(this->_baseSocket, &this->_baseFds);

	std::cout << GREEN << "\n\n0========== SERVER LAUNCHED ==========0" << DEFCOL << std::endl;
	while (!shutServ)
	{
		this->_targetFds = this->_baseFds;
		this->_socketCount = select(FD_SETSIZE, &this->_targetFds, nullptr, nullptr, nullptr);
		if (this->_socketCount == -1)
		{
			if (EINTR)
				break ;
			throw std::invalid_argument(" > Error at select(): ");
		}
		else if (this->_socketCount) { for (int i = 0; i < FD_SETSIZE; ++i) { if (FD_ISSET(i, &this->_targetFds))
		{
			if (i == this->_baseSocket)
				this->newClient(&client_addr, &client_len, &it);
			else
				this->knownClient(it, &i);
		}}}
	}
	it = this->_clients.begin();
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
