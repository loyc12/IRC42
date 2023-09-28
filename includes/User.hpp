#ifndef USER_HPP
# define USER_HPP


# include <iostream>
# include <stdlib.h>		// for which functions ??
# include <fcntl.h> // for file descriptors (?)
# include <sys/socket.h>
# include <netinet/in.h>

# include "IRC.hpp"

class User //								ADD FD INSIDE USER
{
	private:
// 		Data
		int			_fd;
		std::string	_name;
		std::string _userName;
		std::string	_nick;
		std::string _mode;
		std::string _hostname;
		sockaddr_in _client_addr; //when client is connected
		User();

	public:
// 		Constructors & Destructor
		User(sockaddr_in client_addr);
		//User(const User &other);
		//User &operator= (const User &other);
		~User();

// 		Getters - Setters
		int			getFD(void) const;
		std::string	getName(void) const;
		std::string	getNick(void) const;
		sockaddr_in	getClientAddr(void) const;
		std::string getUsername(void) const;
		std::string getMode(void) const;
		std::string getHostname(void) const;

		void		setFD(int fd);
		void		setName(std::string name);
		void		setNick(std::string nick);
		void		setUsername(std::string userName);
		void		setMode(std::string mode);
		void		setHostname(std::string hostname);
		void		parseUserInfo(std::string *args);

};


#endif // USER_HPP
