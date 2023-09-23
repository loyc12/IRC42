#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include "Message.hpp"
# include <map>

class User;
class Message;

class Server
{
	private:
		// Data
		int	_port;
		std::string	_password;
		int	_baseSocket;
		int _newSocket;
		fd_set _fdsMaster;
		fd_set _fdsRead;
		int		_socketCount;
		std::string	_nameServer;
		std::map<int, User*> _clients; //container to store all our clients info
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
		void	start(void);
		void	newClient(struct sockaddr_in *client_addr, socklen_t *client_len, std::map<int, User*>::iterator *it);
		void	knownClient(std::map<int, User*>::iterator it, int *i);
		int		readFromClient(int fd, std::string *message, User *user);
		void	checkPassword(std::string buff, int fd, User* user);
		void	init(void);

};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
