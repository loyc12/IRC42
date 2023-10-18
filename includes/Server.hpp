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

# define CMD_COUNT			10

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

	public:
//		Constructor - Destructor
			Server					(int port, std::string pass);
			~Server					();
// 		Getters - Setters
			const int 				&getPort(void) const;
			const std::string 		&getPass(void) const;
//		CORE (has constructors)
			void	init			(void);
			void	start			(void);
			void	clear			(void);
//		FIND
			bool	isUserInChan	(User *user, Channel *chan);
			bool	checkInvitePerm	(User *user, Channel *chan);
			bool	checkPass		(User *user, Channel *chan, std::string pass);
			bool	checkMaxMbr		(User *user, Channel *chan);
			Channel	*findChannel	(std::string str);
			User	*findUser		(std::string str);
//		CHAN
			void	knownChannel	(User *user, Channel *chan, std::vector<std::string> args);
			void	newChannel		(User *user, std::vector<std::string> args);
			void	dragToChannel	(User *invitee, Channel *chan);
			void	processChanMsg	(User *sender, std::string message, std::vector<std::string> args);
//			void	printClient		(struct sockaddr_in *client_addr); //		DEBUG
//		CLIENT
			void	newClient		(struct sockaddr_in *client_addr, socklen_t *client_len);
			void	knownClient		(int fd);
			void	deleteClient	(int fd);
//		CMDS
			int		checkPassword	(User *user, std::vector<std::string> args);
			int		storeNickname	(User *user, std::vector<std::string> args);
			int		storeUserInfo	(User *user, std::vector<std::string> args);
			int		joinChan		(User *user, std::vector<std::string> args);
			int		leaveChan		(User *user, std::vector<std::string> args);
			int		kickUser		(User *user, std::vector<std::string> args);
			int		quitServer		(User *user, std::vector<std::string> args);
			int		inviteUser		(User *user, std::vector<std::string> args);
			int		setChanTopic	(User *user, std::vector<std::string> args);
			int		setChanMode		(User *user, std::vector<std::string> args);
			int		notACommand		(User *user, std::vector<std::string> args);
			int		getCmdID		(std::string cmd);
			int		execCommand		(User *user, std::vector<std::string> args);
//		FT_I/O
			void	welcomeUser		(User *user);
			void	readFromClient	(User *user, int fd, std::string *lastMsg);
//			void	replyTo			(int target, User *fromUser, User *toUser, std::string code, std::string input);
			void	sendToUser		(User* targetUser, std::string message);

};

#endif // SERVER_HPP
