#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "IRC.hpp"

class Channel {
	private:
		std::string _nameChan;
		std::string _admin; //userName of the creator of channel
		std::vector<std::string> _membersChannel; // could be pointers to users?
		std::string _password;
		Channel(void);
		Channel(Channel const &copy);
		Channel& operator=(Channel const &rhs);

	public:
		Channel(std::string nameChan);
		~Channel(void);

		std::string const &getNameChan(void) const;
		std::string const &getAdmin(void) const;
		std::string const &getPass(void) const;
		void			  setAdmin(std::string const &admin);
		void			  setNameChan(std::string const &chan);
		void			  joinChan(User *user, int fd);
};

#endif