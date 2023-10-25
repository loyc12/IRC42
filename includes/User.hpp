#ifndef USER_HPP
# define USER_HPP

# include "IRC.hpp"

# define CONSTR_USER 	"0========= PARAM-CONSTR(USER) =======0\n"
# define DEST_USER 		"0========== DESTRUCT-(USER) =========0\n"

class Message;

class User //								ADD FD INSIDE USER
{
	private:
// 		Data
		int			_fd;
		std::string	_name;
		std::string _userName;
		std::string	_nick;
		std::string _hostName;
		sockaddr_in _clientAddr; //when client is connected

	public:
		bool wasWelcomed; //				TODO (LL) : use an accessor instead
		std::string	lastMsg;
// 		Constructors & Destructor
		User(sockaddr_in clientAddr);
		~User();

// 		Getters - Setters
		int			getFD			(void) const;
		std::string	getName			(void) const;
		std::string	getNick			(void) const;
		std::string getUsername		(void) const;
		std::string getHostname		(void) const;
		sockaddr_in	getClientAddr	(void) const;

		void		setFD			(int fd);
		void		setName			(std::string name);
		void		setNick			(std::string nick);
		void		setUsername		(std::string userName);
		void		setHostname		(std::string hostName);
		void		setUserInfo		(std::vector<std::string> args);
};


#endif // USER_HPP
