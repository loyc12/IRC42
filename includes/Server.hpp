#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include "Message.hpp"

class User;
class Message;

class Server
{
	private:
		// Data
		int	_port;
		int _newSocketFd;
		int _baseSocketFd;
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
		const int & getBaseSocket(void) const;
		const int & getNewSocket(void) const;
		void		setBaseSocket(int baseSocket);
		void		setNewSocket(int newSocket);

		void initSocket(void);
		void initBind(struct sockaddr_in *server_addr);
		void acceptRequest(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr);
		// receiveMessage(void);
		// sendMessage(void);
};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
