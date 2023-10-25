#include "IRC.hpp"

Server::Server() : shutoff(false)	{debugPrint(YELLOW, CONSTR_SERV); }
Server::~Server() 					{debugPrint(YELLOW, DEST_SERV); }

const int & Server::getPort			(void) const { return (this->_port);}
const int & Server::getPass			(void) const { return (this->_pass);}

void	Server::setPort				(int port) { this->_port = port; }
void	Server::setPass				(int pass) { this->_pass = pass; }


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
	this->_serverAddr.sin_family = AF_INET; //							bind call
	this->_serverAddr.sin_port = htons(this->getPort()); //				IP Adress
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY; //					localhost

//	Connects server's port
	if (bind(this->_baseSocket, (struct sockaddr *) &this->_serverAddr, sizeof(this->_serverAddr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

//	Sets to receive all conections
	listen(this->_baseSocket, SOMAXCONN);
	FD_ZERO(&this->_baseFds);
	FD_SET(this->_baseSocket, &this->_baseFds);

	debugPrint(GREEN, LAUNCH); //										DEBUG
}



//	LAUNCHES THE SERVER AND ITS PROCESSES
void	Server::run(void)
{
	struct sockaddr_in				client_addr;
	socklen_t 						client_len = sizeof(client_addr);

	this->init();

	while (!this->shutoff)
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
						this->knownClient(clientFd);
				}
				if (this->shutoff)
					break;
			}
		}
	}
}


//	CLOSES THE SERVER SAFELY
void	Server::clear(void)
{
	debugPrint(MAGENTA, CLOSING); //					DEBUG

	for (std::map<std::string, Channel*>::iterator it = this->_chanContainer.begin(); it != this->_chanContainer.end(); it++)
		delete it->second;
	this->_chanContainer.clear();
	for (std::map<int, User*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		delete it->second;

	this->_clients.clear();
	close(this->_baseSocket);
	close(this->_newSocket);

	delete this;
}
