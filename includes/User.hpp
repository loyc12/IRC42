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
//		int			_fd;
		std::string	_name;
		std::string _userName;
		std::string	_nick;
		std::string _mode;
		std::string _hostname;
		sockaddr_in _client_addr; //when client is connected

	public:
// 		Constructors & Destructor
		User(sockaddr_in client_addr);
		~User();

// 		Getters - Setters
		std::string	getName(void) const;
		std::string	getNick(void) const;
		sockaddr_in	getClientAddr(void) const;
		std::string getUsername(void) const;
		std::string getMode(void) const;
		std::string getHostname(void) const;

		void		setName(std::string name);
		void		setNick(std::string nick);
		void		setUsername(std::string userName);
		void		setMode(std::string mode);
		void		setHostname(std::string hostname);
		void		parseUserInfo(std::string *args);

		void 	debugPrint(std::string color, std::string message);
};


#endif // USER_HPP
