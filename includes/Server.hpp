#ifndef SERVER_HPP
# define SERVER_HPP

# include "IRC.hpp"

# define CONSTR_SERV		"0======= PARAM-CONSTR(SERVER) ========0\n"
# define DEST_SERV			"0========= DESTRUCT-(SERVER) =========0\n"
# define LAUNCH				"\n0========== SERVER LAUNCHED ==========0\n"
# define CONNECTED			"\n0========= CLIENT CONNECTION =========0\n"
# define DISCONNECTED		"\n0======== CLIENT DISCONNECTED ========0\n"
# define DENIED				"\n0========= CONNECTION DENIED =========0\n"


//ENTRY CODE
# define RPL_WELCOME		" 001"

//CHANNEL CODE
# define ERR_NOSUCHCHANNEL	"403" //	chan does not exist

# define RPL_NOTOPIC		"331" //	no topic set for chan
# define RPL_TOPIC			"332" //	topic of the chan
# define RPL_NAMREPLY		"353" //	list of nicknames in channel

class User;
class Channel;
class Message;

class Server
{
	private:
//		Parsing
			int			_port;
			std::string	_password; //					UNUSED ???
//		Start
			int			_baseSocket;
			int 		_newSocket;
			int			_socketCount;
			fd_set 		_baseFds;
			fd_set 		_targetFds;
//		Flag
			bool		_welcomeFlag;
			bool		_isSet;
//		Storage
			std::map<int, User*> 			_clients;
			std::map<std::string, Channel*> _chanContainer;
			struct sockaddr_in				_serverAddr;
//		Utils
			std::map<int, User*>::iterator	_it;


	public:
//		Constructor - Destructor
			Server(int port); //					TAKE PASS AS ARG (?)
			~Server();

// 		Getters - Setters
			const int 			&getPort(void) const;
			const std::string 	&getPass(void) const;

//		FT_PASS
			int 	checkPassword(User *user, std::string pass);
			int		badPassword(User* user);
//		FT_SERVER
			void	initServ(void);
			void	start(void);
			void	clearServ(void);
//		FT_CLIENT
			void	newClient(struct sockaddr_in *client_addr, socklen_t *client_len);
			void	knownClient(int *clientFd);
			int		deleteClient(int fd, char *buff);
			void	printClient(struct sockaddr_in *client_addr);
//		FT_I/O
			int		command(std::string	*args);
			int		readFromClient(User *user, std::string *message);
			void	sendToClient(User *user, std::string message);
			void 	respondToClient(User* user, std::string code, std::string message);
//		FT_STORAGE
			void	storeNickname(User *user, std::string *args);
//		UTIL
			void	manageJoinCmd(User *user, std::string *args);
			void	welcomeMsg(User *user);


};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
