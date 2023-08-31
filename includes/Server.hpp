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
		int	port;
		int	pass;

		// Private Constructor
		Server();

	public:
		// Constructors & Destructor
		Server(int _port, int _pass);
		Server(const Server &other);
		Server &operator= (const Server &other);
		~Server();

		// Getters - Setters
		int	getPort(void) const ;
		int	getPass(void) const;
};

std::ostream &operator<< (std::ostream &out, const Server &rhs);

#endif // SERVER_HPP
