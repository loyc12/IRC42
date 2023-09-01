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

};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP