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
