#include "IRC.hpp"

//	PARSE THE DATA FROM A NEW CLIENT'S FIRST MESSAGES
void	Server::newClient(struct sockaddr_in *client_addr, socklen_t *client_len)
{
//	Generates a new socket connection to adds to baseSocket
	this->_newSocket = accept(this->_baseSocket, (struct sockaddr *)&*client_addr, &*client_len);

	if (this->_newSocket <= 0)
		throw std::invalid_argument(" > error at accept(): ");
//	Creates a new user for this new Socket & stores User container inside the Server
	else
	{
		User* user = new User(*client_addr);
		user->setFD(this->_newSocket);
		this->_clients.insert(std::pair<int, User*>(this->_newSocket, user));
		FD_SET(this->_newSocket, &this->_baseFds);
	}
}

//	READS AN INCOMING MESSAGE FROM A ALREADY EXISTING CLIENT
void	Server::knownClient(int fd)
{
	std::map<int, User*>::iterator it = this->_clients.find(fd);

//	Finds target client
	if (it != this->_clients.end())
	{
		User *user = it->second;
		if (user != NULL)
			readFromClient(user, fd);
	}
}

//	DELETES A GIVEN CLIENT
void Server::deleteClient(int fd)
{
//	Sets iterator to the client's Fd
	std::map<int, User*>::iterator it = this->_clients.find(fd);

//	Make the user leave all channels first
	for (std::map<std::string, Channel*>::iterator ite = this->_chanContainer.begin(); ite != this->_chanContainer.end(); ite++)
	{
		if (ite->second->hasMember(it->second))
			removeFromChan(it->second, NULL, ite->second);
	}

//	Deletes the associated user instance
	if (it != this->_clients.end())
	{
		delete it->second;

	//	Clears its fd
		this->_clients.erase(fd);
		debugPrint(CYAN, DISCONNECTED);

	//	Removes its fd from _baseFds
		close(fd);
		FD_CLR(fd, &(this->_baseFds));
	}
	else
		std::cerr << "invalid fd : cannot delete this client" << std::endl;
}
