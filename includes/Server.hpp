#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include "Message.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include <map>

//ENTRY CODE
# define RPL_WELCOME " 001"

//CHANNEL CODE
# define ERR_NOSUCHCHANNEL "403" //chan does not exist


# define RPL_NOTOPIC "331" //no topic set for chan
# define RPL_TOPIC "332" //topic of the chan
# define RPL_NAMREPLY "353" //list of nicknames in channel

class User;
class Channel;
class Message;

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
		int		disconnectClient(char *buff, int fd);
		void	knownClient(std::map<int, User*>::iterator it, int *i);
		int		readFromClient(int fd, std::string *message, User *user);

		void	sendToClient(User *user, int fd, std::string message);
		void 	responseToClient(User* user, int fd, std::string code, std::string message);
		int		badPassword(int fd, User* user);
		int 	checkPassword(std::string buff, int fd, User* user);
		void	checkNickname(std::string *args, User *user, int fd);
		void	init(void);
		void	manageJoinCmd(std::string *args, User *user, int fd);
		void	welcomeMsg(User *user, int fd);
		void 	debugPrint(std::string color, std::string message);

};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
