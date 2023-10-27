#ifndef SERVER_HPP
# define SERVER_HPP

# include "IRC.hpp"

# define CONSTR_SERV 		"0=========== CONSTR-(SERVER) =========0\n"
# define DEST_SERV 			"0========== DESTRUCT-(SERVER) ========0\n"
# define LAUNCH 			"\n0=========== SERVER LAUNCHED =========0\n\n"
# define CONNECTED 			"\n0========== CLIENT CONNECTED =========0\n"
# define DISCONNECTED 		"\n0======== CLIENT DISCONNECTED ========0\n\n"
# define DENIED 			"\n0========= CONNECTION DENIED =========0\n"
# define CLOSING 			"\n0=========== CLOSING SERVER ==========0\n\n"
# define WELCOME_HEADER 	"Welcome to our IRC server!"

# define CMD_COUNT				12
# define REQUEST				0
# define CHAN					1


class Server
{
	private:
//		Parsing
			int		_port;
			int		_pass;
//		Start
			int		_baseSocket;
			int 	_newSocket;
			int		_socketCount;
			fd_set 	_baseFds;
			fd_set 	_targetFds;
//		Storage
			std::map<int, User*> 			_clients;
			std::map<std::string, Channel*> _chanContainer;
			struct sockaddr_in				_serverAddr;

	public:
			bool					shutoff;
//		Constructor - Destructor
			Server					();
			~Server					();
// 		Getters - Setters
			const int 				&getPort(void) const;
			const int 				&getPass(void) const;
			void					setPort(int port);
			void					setPass(int pass);
//		CORE (has constructors)
			void	init			(void);
			void	run				(void);
			void	clear			(void);
//		CHAN
			void	knownChannel	(User *user, Channel *chan, std::vector<std::string> args);
			void	newChannel		(User *user, std::vector<std::string> args);
			void	dragToChannel	(User *invitee, Channel *chan);
			void	processChanMsg	(User *sender, std::vector<std::string> args);
			void	processPrivMsg	(User *user, std::vector<std::string> args);
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
			int		sendMessage		(User *user, std::vector<std::string> args);
			int		getCmdID		(User *user, std::string cmd);
			int		execCommand		(User *user, std::vector<std::string> args);
			int		notACommand		(User *user, std::vector<std::string> args);
			int		notLoggedIn		(User *user, std::vector<std::string> args);
//		COMM
			void	welcomeUser		(User *user);
			void	readFromClient	(User *user, int fd);
//		FIND
			bool	isUserInChan	(User *user, Channel *chan);
			bool	checkInvitePerm	(User *user, Channel *chan);
			bool	checkPass		(User *user, Channel *chan, std::string pass);
			bool	checkMaxMbr		(User *user, Channel *chan);
			bool	isNickValid		(User *user, std::string nickname);
			bool	isMsgEnd		(std::string str);
			Channel	*findChannel	(std::string str);
			User	*findUser		(std::string str);
			int		closeServer		(User *user, std::vector<std::string> args);//DEBUG
};

#endif // SERVER_HPP
