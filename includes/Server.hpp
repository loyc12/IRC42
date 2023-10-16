#ifndef SERVER_HPP
# define SERVER_HPP

# include "IRC.hpp"

# define CONSTR_SERV 		"0========= PARAM-CONSTR(SERVER) ======0\n"
# define DEST_SERV 			"0========== DESTRUCT-(SERVER) ========0\n"
# define LAUNCH 			"\n0========== SERVER LAUNCHED ==========0\n\n"
# define CONNECTED 			"\n0========== CLIENT CONNECTED =========0\n"
# define DISCONNECTED 		"\n0========= CLIENT DICONNECTED ========0\n\n"
# define DENIED 			"\n0========= CONNECTION DENIED =========0\n"
# define CLOSING 			"\n0=========== CLOSING SERVER ==========0\n\n"
# define WELCOME_HEADER 	"Welcome to this IRC server"

//ENTRY CODE
# define RPL_WELCOME			"001"
//CHANNEL CODE
# define ERR_NOSUCHCHANNEL		"403"
# define ERR_NEEDMOREPARAMS		"461"
# define ERR_ALREADYREGISTRED	"462"
# define ERR_PASSWDMISMATCH		"464"
# define ERR_CHANNELISFULL		"471"
# define ERR_INVITEONLYCHAN		"473"




# define RPL_NOTOPIC			"331" //no topic set for chan
# define RPL_TOPIC				"332" //topic of the chan
# define RPL_NAMREPLY			"353" //list of nicknames in channel
# define RPL_REPLY				"302" //Reply Mode
# define JOIN					"JOIN"
# define REQUEST				0 //	NOTE : split replyTo() into two functions instead?
# define CHAN					1


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
//		Storage
			std::map<int, User*> 			_clients;
			std::map<std::string, Channel*> _chanContainer;
			struct sockaddr_in				_serverAddr;
//		Utils
//			std::map<int, User*>::iterator	_it;


	public:
//		Constructor - Destructor
			Server(int port, std::string pass); //					TAKE PASS AS ARG (?)
			~Server();
// 		Getters - Setters
			const int 			& getPort(void) const;
			const std::string 	& getPass(void) const;


//		FT_CHECK
			bool	isUserInChan	(User *user, Channel *chan);
			bool	checkInvitePerm	(User *user, Channel *chan);
			bool	checkPass		(User *user, Channel *chan, std::string pass);
			bool	checkMaxMbr		(User *user, Channel *chan);
			void	knownChannel	(User *user, Channel *chan, std::vector<std::string> args);
			void	newChannel		(User *user, std::vector<std::string> args);

//		FT_CMD
			int		checkPassword	(User *user, std::vector<std::string> args);
			int		storeNickname	(User *user, std::vector<std::string> args);
			int		storeUserInfo	(User *user, std::vector<std::string> args);
			int		cmdJoin			(User *user, std::vector<std::string> args);
			int		kickUser		(User *user, std::vector<std::string> args);
			int		inviteUser		(User *user, std::vector<std::string> args);
			int		setChannelTopic	(User *user, std::vector<std::string> args);
			int		Mode			(User *user, std::vector<std::string> args);
//			int		setUserMode		(User *user, std::vector<std::string> args, Channel *channel, std::string code);
			int		processMessage	(User *user, std::vector<std::string> args);
			int		quitServer		(User *user, std::vector<std::string> args);
			int		getCmdID		(std::string cmd);
			int		execCommand		(User *user, std::vector<std::string> args);
//		FT_I/O
			void	welcomeUser		(User *user);
			void	replyTo			(int target, User *fromUser, User *toUser, std::string code, std::string input);
//			void	reply			(User *user, std::string code, std::string input);
			Channel	*findChannel	(std::string str);
			void	sendToChan		(User *fromUser, std::string message, std::vector<std::string> args);
			void	readFromClient	(User *user, int fd, std::string *lastMsg);
//		FT_CLIENT
			void	printClient		(struct sockaddr_in *client_addr);
			void	newClient		(struct sockaddr_in *client_addr, socklen_t *client_len);
			void	knownClient		(int fd);
			void	deleteClient	(int fd);
//		FT_SERVER
			void	init	(void);
			void	start	(void);
			void	clear	(void);

};

#endif // SERVER_HPP
