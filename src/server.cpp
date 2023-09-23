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
void Server::checkPassword(std::string pass, int fd, User* user)
{
	//starting cleanup
	(void)user;
	int ret;

	if (pass.compare(this->getPass()) != 0)//check si c'est le bon mot de passe
	{
		//task = select et/ou voir le code quand un client deconnecte, probablement cela qu'il faut faire TODO : closing its FD and telling them to fuck off
		std::cout << std::endl << RED << "0========= CONNECTION DENIED =========0" << DEFCOL << std::endl;
		std::string errMsg = "Incorrect password";
		std::string response = ":" + this->getNameServer() + " " + std::to_string(403) + " " + user->getNick() + " :" + errMsg + "\r\n"; //TODO create a enum or list of status code/numeric codes

    	// Send the error message to the LimeChat client
		send(fd, response.c_str(), response.size(), 0);
		close(fd);//a enlever eventuellement, autre solution.
		std::map<int, User*>::iterator it = this->_clients.find(fd);

		if (it != this->_clients.end())
			delete it->second;

		this->_clients.erase(fd);

		std::cout << std::endl << std::endl;
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
	{
		bzero(buff, BUFFSIZE);
		std::cout << std::endl << CYAN << "0======== CLIENT DISCONNECTED ========0" << DEFCOL << std::endl << std::endl;
		//delete client from container
		std::map<int, User*>::iterator it = this->_clients.find(fd);

		if (it != this->_clients.end())
			delete it->second;

		this->_clients.erase(fd);

		std::cout << std::endl << std::endl;
		return (-1);
	}
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
				this->checkPassword(args[1], fd, user);
				break;
			case 1:
				user->setNick(args[1]);
				std::cout << "nickname: " << user->getNick() << std::endl;
				break;
			case 2:
				std::cout << "will do stuff for user" << std::endl; //
				break;
			case 3:
				std::cout << "do stuff for join" << std::endl;
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

void	Server::init()
{
	struct sockaddr_in	server_addr;

//	Inits base socket
	this->_baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");

//	Makes it so socket can be reused if available
	const int reuse = 1;
	if (setsockopt(this->_baseSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		throw std::invalid_argument(" > Error at setsocketopt(): ");

//	Prepares args for bind() call
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //					bind call
	server_addr.sin_port = htons(this->getPort()); //		conversion to network byte order (Ip adress)
	server_addr.sin_addr.s_addr = INADDR_ANY; //			host ip adress **INADDR_ANY go get localhost

//	Connects to the server's port
	if (bind(this->_baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

//	Sets up baseSocket to receive all connections
	listen(this->_baseSocket, SOMAXCONN);

}

void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len, std::map<int, User*>::iterator *it){

	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *) &*client_addr, &*client_len);
	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > Error at accept(): ");
	else
	{
		std::cout << CYAN << "\n0========== CLIENT CONNECTED =========0\n" << " > on socket : "
		<< this->_newSocket << " " << inet_ntoa(client_addr->sin_addr)
		<< ":" << ntohs(client_addr->sin_port) << DEFCOL << "\n\n" << std::endl;

//	Creation de l'objet de User
		User* user = new User(*client_addr);

		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		*it = this->_clients.find(this->_newSocket);
		FD_SET(this->_newSocket, &this->_fdsMaster);
	}
}

void	Server::knownClient(std::map<int, User*>::iterator it, int *i){

	std::string	message;

	if (it != this->_clients.end()){

		User* userPtr = it->second;
		if (readFromClient(*i, &message, userPtr) < 0) {
			close(*i);
			FD_CLR(*i, &this->_fdsMaster);
		}
	}
}

void	Server::start(void){

	struct sockaddr_in	client_addr;
	socklen_t 			client_len = sizeof(client_addr);
	std::map<int, User*>::iterator it;

//	Setup our server (binding and socket)
	this->init();

//	Prepares fds for select
	FD_ZERO(&this->_fdsMaster);
	FD_SET(this->_baseSocket, &this->_fdsMaster);

	std::cout << GREEN << "\n\n0========== SERVER LAUNCHED ==========0" << DEFCOL << std::endl;
//	Client interaction loop
//[] stopFlag ?--> self descriptive ?
	while (!stopFlag)
	{
		this->_fdsRead = this->_fdsMaster;

//		will need to check for active and non-active socket... //TODO fix that so when we close a client fd when incorrect password does not crash server
		this->_socketCount = select(FD_SETSIZE, &this->_fdsRead, nullptr, nullptr, nullptr);

		if (stopFlag)//bcs of this
			break;
		//[] need description
		if (this->_socketCount == -1)
			throw std::invalid_argument(" > Error at select(): ");
		//[] need description
		else if (this->_socketCount) { for (int i = 0; i < FD_SETSIZE; ++i) { if (FD_ISSET(i, &this->_fdsRead))
		{
			if (i == this->_baseSocket)
				this->newClient(&client_addr, &client_len, &it);
			else
				this->knownClient(it, &i);//problem here
		}}}
	}
	//new to call delete user? to be checked with leaks
	close(this->_baseSocket);
	close(this->_newSocket);
}
