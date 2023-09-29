#ifndef SERVER_HPP
# define SERVER_HPP

# include "IRC.hpp"

# define CONSTR_SERV 		"0========= PARAM-CONSTR(SERVER) ======0\n"
# define DEST_SERV 			"0========== DESTRUCT-(SERVER) ========0\n"
# define LAUNCH 			"\n0========== SERVER LAUNCHED ==========0\n"
# define CONNECTED 			"\n0========== CLIENT CONNECTED =========0\n"
# define DISCONNECTED 		"\n0========= CLIENT DICONNECTED ========0\n\n"
# define DENIED 			"\n0========= CONNECTION DENIED =========0\n"

//ENTRY CODE
# define RPL_WELCOME		" 001"
//CHANNEL CODE
# define ERR_NOSUCHCHANNEL	"403" //chan does not exist
# define RPL_NOTOPIC		"331" //no topic set for chan
# define RPL_TOPIC			"332" //topic of the chan
# define RPL_NAMREPLY		"353" //list of nicknames in channel

class Server
{
	private:
//		Parsing
			int			_port;
			std::string	_password;
//		Start
			int			_baseSocket;
			int 		_newSocket;
			int			_socketCount;
			fd_set 		_baseFds;
			fd_set 		_targetFds;
//		Flag
			bool		_welcomeFlag;
			bool		_isSet;
//			bool		_isMsg;
//		Storage
			std::map<int, User*> 			_clients;
			std::map<std::string, Channel*> _chanContainer;
			struct sockaddr_in				_serverAddr;
//		Utils
			std::map<int, User*>::iterator	_it;


	public:
//		Constructor - Destructor
			Server(int port);
			~Server();

// 		Getters - Setters
			const int 			& getPort(void) const;
			const std::string 	& getPass(void) const;


//		FT_SERVER
			void	initServ(void);
			void	start(void);
			void	clearServ(void);
//		FT_CLIENT
			void	newClient(struct sockaddr_in *client_addr, socklen_t *client_len);
			void	knownClient(int *clientFd);
			int		deleteClient(int fd, char *buff);
			void	printClient(struct sockaddr_in *client_addr);
//		FT_CMD
			int		getCmdID(std::string cmd);
			int		execCommand(User *user, int fd, std::string *args);
			int		checkPassword(User *user, int fd, std::string *args);
			int		storeNickname(User *user, int fd, std::string *args);
			int		storeUserInfo(User *user, int fd, std::string *args);
			int		joinChannel(User *user, int fd, std::string *args);
			int		kickUser(User *user, int fd, std::string *args);
			int		inviteUser(User *user, int fd, std::string *args);
			int		setChannelTopic(User *user, int fd, std::string *args);
			int		setUserMode(User *user, int fd, std::string *args);
			int		processMessage(User *user, int fd, std::string *args);
//		FT_I/O
			void	sendToClient(User *user, int fd, std::string message);
			void	respondToClient(User* user, int fd, std::string code, std::string input);
			void	welcomeMsg(User *user, int fd);
			int		readFromClient(User *user, int fd, std::string *last_msg);

};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
