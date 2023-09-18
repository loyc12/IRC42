#include "IRC.hpp"

//added them in Server.hpp
static int newSocket = 0;
static int baseSocket = 0;

/**
 * Function to switch our global bool (flag) to stop the infinite loop
 * The infinite loop to keep our IRC server running
 * @param sig
 */
static void	stop(int sig)
{
//	Switchs our global bool to stop the infinite loop
	(void)sig;
	stopFlag = true;
	std::cout << std::endl << std::endl << MAGENTA << " > Closing and cleaning ..." << DEFCOL << std::endl << std::endl;
//	exit(1); //	here because commands are blocking, preventing flag checks
}

void checkPassword(char *buff, Server *server, int fd /* User *client */)
{
	//PASS 5645 <- client send password like this
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
	if (pass.compare(server->getPass()) != 0)
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

int read_from_client(int fd, std::string *message, Server *server, User *user)
{
	(void)user;
	char 		buff[BUFFSIZE];
	bzero(buff, BUFFSIZE);
	int byteReceived = recv(fd, buff, BUFFSIZE - 1, 0);
	if (byteReceived <= 0)
	{
		bzero(buff, BUFFSIZE);
		std::cout << std::endl << CYAN << "0======== CLIENT DISCONNECTED ========0" << DEFCOL << std::endl << std::endl;
		//will need to delete our client from map. Even check if it was already in the container
		/*
		server->_clients(fd) //might need an error check here if it was not there, erase will not work...
		*/
		return (-1);
	}
	else if (byteReceived)
	{
		//int ret;
		std::string tmp = buff;
		if (tmp.find("PASS ") != std::string::npos)
			checkPassword(buff, server, fd /* client (instance of User)*/);
		message->assign(buff, 0, byteReceived);
		std::cout << *message;
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


// void	checkBuff(char *buff, Server *server)
// {
// 	/*
// 	if (checkPassword == -1)
// 		throw error here: invalid password
// 	if (buff == command) //KICK, INVITE, TOPIC or MODE with flags -> i t k o l
// 		do the command->need to make sure that the user has the necessary requirements (admin for example)
// 	else
// 		send msg to the console of client(s)
// 		and our terminal??

// 	*/
// 	std::string buf = buff;
// 	if (checkPassword(buff, server) == -1)
// 		throw std::invalid_argument(" > Error: invalid password");
// 	if (buf.compare("KICK") || buf.compare("INVITE") || buf.compare("TOPIC") || buf.compare("MODE"))
// 		std::cout << "do the command if you have requirements" << std::endl;
// 	else
// 		std::cout << buf << std::endl;
// }

/*
void irc(Server *server) //! If we want server to have access to the container _clients, will need to think about adding this function into server class...
{
	fd_set				fdsMaster, fdsRead; //, fdsWrite;
	int 				socketCount;
	std::string 		message;
	struct sockaddr_in	server_addr, client_addr;
	socklen_t 			client_len = sizeof(client_addr);

//	Inits base socket
	baseSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (baseSocket < 0)
		throw std::invalid_argument(" > Error at socket(): ");

//	Makes it so socket can be reused if available
	const int reuse = 1;
	if (setsockopt(baseSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
		throw std::invalid_argument(" > Error at setsocketopt(): ");

//	Prepares args for bind() call
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //					bind call
	server_addr.sin_port = htons(server->getPort()); //		conversion to network byte order (Ip adress)
	server_addr.sin_addr.s_addr = INADDR_ANY; //			host ip adress **INADDR_ANY go get localhost

//	Connects to the server's port
	if (bind(baseSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		throw std::invalid_argument(" > Error at bind(): ");

//	Sets up baseSocket to receive all connections
	listen(baseSocket, SOMAXCONN);

//	Prepares fds for select
	FD_ZERO(&fdsMaster);
	FD_SET(baseSocket, &fdsMaster);

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
			if (i == baseSocket) //Connection request on base socket
			{
				newSocket = accept(baseSocket, (struct sockaddr *) &client_addr, &client_len);

				if (newSocket <= 0)
					throw std::invalid_argument(" > Error at accept(): ");
				else
				{
					std::cout << std::endl << CYAN << "0========== CLIENT CONNECTED =========0" << std::endl
					<< " > on socket : " << newSocket << " " << inet_ntoa(client_addr.sin_addr)
					<< ":" << ntohs(client_addr.sin_port) << DEFCOL << std::endl << std::endl;
					User user(client_addr);//new instance of class User: store the info on client_addr.sin_port
					server->_clients[newSocket] = user;
					//server->_clients.insert(std::pair<int, User*>(newSocket, user));
					FD_SET(newSocket, &fdsMaster);
				}
			}
			else //	Reads messages from a known client
			{
				if (read_from_client(i, &message, server, user) < 0) //	do else if () instead (?)
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
		}}}
	}
	close(baseSocket);
	close(newSocket);
}
*/

int main(int ac, char **av)
{
	signal(SIGQUIT, SIG_IGN); //ignore ctrl-backslash
	signal(SIGINT, stop);

	std::cout << DEFCOL;
	try
	{
		if (ac != 3)
			throw std::invalid_argument(" > Error main(): Invalid argument count.");

 //       std::string firstArg = av[1];
		std::size_t found = std::string(av[1]).find_first_not_of("1234567890");
		if (found != std::string::npos)
			throw std::invalid_argument(" > Error main(): Not a port");

		int port = atoi(av[1]);
		if (port < 6660 || 6669 < port)
			throw std::invalid_argument(" > Error main(): Not a TCP port for IRC");
		if (port < 1025 || 65535 < port)
			throw std::invalid_argument(" > Error main(): Invalid port");

		std::string password = av[2];
		Server server(port);
		if (password.compare(server.getPass()) != 0)
			throw std::invalid_argument(" > Error main(): Invalid password");

		std::cout << std::endl; //  DEBUG
		server.irc();
	}
	catch (std::exception &e)
	{
		std::cerr << std::endl << std::endl << RED << e.what() << DEFCOL ;

		if (errno)
			std::cout << RED << std::strerror(errno) << DEFCOL << std::endl;

		close (baseSocket);
		close (newSocket);

		std::cout << std::endl << std::endl; // DEBUG

		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
