#include "IRC.hpp"

// 0================ BASE FUNCTIONS ================0
//private
Server::Server() : _port(6667), _password("1234"), _baseSocket(0), _newSocket(0){
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

// 0================ OTHER FUNCTIONS ================0

void Server::checkPassword(char *buff, int fd, User* user)
{
	//PASS 5645 <- client send password like this
	(void)user;
	int ret;
	std::string buf = buff;
	size_t i = 0;
	while (i < buf.length())
	{
		if (buf.compare("PASS ") == 0) //loop to make sure that you get PASS
			break;
		i++;
	}
	std::string pass = buf.substr(5, 4); //isolate the password sent by client ***HARD CODE here
	if (pass.compare(this->getPass()) != 0)
		throw std::invalid_argument(" > Error: invalid password");
		//return (-1);
	else //	 ----------------------------------------------------------------------------------------------------------- WELCOME MESSAGE HERE
	{
		//----add the client to our container (as it's a legit client)----//
		//server->_clients.insert(std::pair<int, User*>(fd, client));
		/*--NOW, we can use our container to have access to ALL the info of our client to send the msg ---*/
		/*oss << ":" << m_hostname << " 001 " << m_userDB[fd].m_nickname << " :Welcome to the IRCServ, " << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username << "@" << m_hostname << "\r\n";*/
		std::ostringstream ss;
		ss << GREEN << "Welcome to this IRC server!" << NOCOLOR << "\r\n";
		std::string welcome = ss.str();

		ret = send(fd, welcome.c_str(), welcome.size(), 0);
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
		//std::cout << GREEN << "Welcome to this IRC server!" << NOCOLOR << std::endl;
	}
	//return (0);
}

int	Server::read_from_client(int fd, std::string *message, User *user)
{
	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);
	if (byteReceived <= 0)
	{
		bzero(buff, BUFFSIZE);
		std::cout << std::endl << CYAN << "0======== CLIENT DISCONNECTED ========0" << DEFCOL << std::endl << std::endl;
		//will need to delete our client from map. Even check if it was already in the container
		std::map<int, User*>::iterator it = this->_clients.find(fd);
		if (it != this->_clients.end())
			delete it->second;
		this->_clients.erase(fd);
		return (-1);
	}
	else if (byteReceived)
	{
		//int ret;
		std::string tmp = buff;
		if (tmp.find("PASS ") != std::string::npos)
			this->checkPassword(buff, fd, user);
		message->assign(buff, 0, byteReceived);
		std::cout << *message;
		if (tmp.find("NICK ") != std::string::npos)
		{
			std::string tmp2 = tmp.substr(5, message->length());
			user->setNick(tmp2);
			std::cout << "nickname: " << user->getNick() << std::endl;
		}
		for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
			std::cout << it->first << " => " << it->second->getNick() << std::endl;
		/*USER login name 0(no idea what's that) *(no idea) realname */
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

void	Server::irc(void){

	fd_set				fdsMaster, fdsRead; //, fdsWrite;
	int 				socketCount;
	std::string 		message;
	struct sockaddr_in	server_addr, client_addr;
	socklen_t 			client_len = sizeof(client_addr);
	std::map<int, User*>::iterator it;

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

//	Prepares fds for select
	FD_ZERO(&fdsMaster);
	FD_SET(this->_baseSocket, &fdsMaster);

//	Client interaction loop
	while (!stopFlag)
	{
		fdsRead = fdsMaster;
//		fdsWrite = fdsMaster; // 	for eventual reading; third argument of select()

//		still blocking ... but that's normal ???
		socketCount = select(FD_SETSIZE, &fdsRead, nullptr, nullptr, nullptr);

		if (stopFlag)
			break;
		if (socketCount == -1)
			throw std::invalid_argument(" > Error at select(): ");
		else if (socketCount) { for (int i = 0; i < FD_SETSIZE; ++i) { if (FD_ISSET(i, &fdsRead))
		{
			if (i == this->_baseSocket) /*Connection request on base socket*/
			{
				this->_newSocket = accept(this->_baseSocket, (struct sockaddr *) &client_addr, &client_len);

				if (this->_newSocket <= 0)
					throw std::invalid_argument(" > Error at accept(): ");
				else
				{
					std::cout << std::endl << CYAN << "0========== CLIENT CONNECTED =========0" << std::endl
					<< " > on socket : " << this->_newSocket << " " << inet_ntoa(client_addr.sin_addr)
					<< ":" << ntohs(client_addr.sin_port) << DEFCOL << std::endl << std::endl;
					User* user = new User(client_addr);//new instance of class User: store the info on client_addr.sin_port
					// this->_clients[this->_newSocket] = user;
					this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
					it = this->_clients.find(this->_newSocket);
					FD_SET(this->_newSocket, &fdsMaster);
				}
			}
			else //	Reads messages from a known client
			{
				//need to send the correct pointer
				if (it != this->_clients.end()){
					User* userPtr = it->second;
					if (read_from_client(i, &message, userPtr) < 0) //	do else if () instead (?)
					{
						close(i);
						FD_CLR(i, &fdsMaster);
					}
					// if (accept_from_client(i, &message, server) < 0)
					// {
					// 	//Parsing de commande (mdp)
					// }
					// if (send_to_clients(i, &message, server) < 0)
					// {
					// 	//Envoyer aux destinataires voulus.
					// }
				}
			}
		}}}
	}
	//new to call delete user? to be checked with leaks
	close(this->_baseSocket);
	close(this->_newSocket);
}