#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>

# include "IRC.hpp"

class Server
{
	private:
		// Data
		int			_port;
		std::string	_password;
		int			_baseSocket;
		int 		_newSocket;
		fd_set 		_baseFds;
		fd_set 		_targetFds;
		int			_socketCount;
		bool		_welcomeFlag;
		bool		_isSet;
		std::string	_nameServer;
		std::map<int, User*> _clients; //container to store all our clients info
		std::map<std::string, Channel*> _chanContainer; //have all the channels that exist on our server
		// Private Constructor
		Server();

	public:
		// Constructors & Destructor
		Server(int port);
		Server(const Server &other);
		Server &operator= (const Server &other);
		~Server();

		// Getters - Setters
		const int &	getPort(void) const ;
		const std::string & getPass(void) const;
		const std::string & getNameServer(void) const;

		// Others functions
		void	start(void);
		void	newClient(struct sockaddr_in *client_addr, socklen_t *client_len, std::map<int, User*>::iterator *it);
		int		disconnectClient(User *user, char *buff);
		void	knownClient(std::map<int, User*>::iterator it, int *i);
		int		readFromClient(User *user, std::string *message);

		void	sendToClient(User *user, std::string message);
		void 	responseToClient(User* user, std::string code, std::string message);
		int		badPassword(User* user);
		int 	checkPassword(User *user, std::string pass);
		void	checkNickname(User *user, std::string *args);
		void	init(void);
		void	manageJoinCmd(std::string *args, User *user);
		void	welcomeMsg(User *user);
};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
