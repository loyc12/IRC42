#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "IRC.hpp"

# define CONSTR_CHAN 	"0======= PARAM-CONSTR(CHANNEL) ======0\n"
# define DEST_CHAN 		"0======== DESTRUCT-(CHANNEL) ========0\n"

class Channel
{
	private:
		std::string _chanName;
		std::string _adminName; //						userName of the creator of channel
		std::vector<std::string> _membersChannel; //	could be pointers to users?
		std::string _password;

	public:
		Channel(std::string chanName);
		~Channel(void);

		std::string const &getChanName	(void) const;
		std::string const &getAdminName	(void) const;
		std::string const &getPass		(void) const;

		void			  setChanName	(std::string const &chan);
		void			  setAdminName	(std::string const &admin);

		void			  joinChan		(User *user, int fd);
};

#endif