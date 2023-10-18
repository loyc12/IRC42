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
		std::vector<User*> _chanMembers; //				THIS IS A LIST NOW
		std::vector<User*> _chanOps; //				THIS IS A LIST NOW
		std::string	_topic;

		bool		_isInviteOnly;
		std::string _password;
		int			_maxMemberCount;

	public:
		Channel(std::string chanName);
		~Channel(void);

		std::string const &getChanName			(void) const;
		std::string const &getAdminName			(void) const;
		std::string const &getPass				(void) const;
		std::string const &getTopic				(void) const;
		int			const &getMaxMbrCnt			(void) const;
		int				   getMemberCnt			(void) const;
		bool		const &getInviteFlag 		(void) const;

		void	setChanName						(std::string const &chan);
		void	setAdminName					(std::string const &admin);
		void	setPass							(std::string const &password);
		void	setTopic						(std::string const &topic);
		void	setMaxMemberCount				(int const &count);
		void	setInviteFlag					(bool const &boolean);

		bool	isSameUser						(User *user1, User *user2);
		bool	hasMember						(User *user);
		void	addMember						(User *user);
		void	addChanOps						(User *user);
		void	removeMember					(User *user);
		User 	*getMember						(int i);
//		void	replyToChan						(User *user, std::string code, std::string input);
		void	updateMemberList				(User *user);
		void	sendToChan						(User *sender, std::string message, bool sendToSender);
		void	sendToChan						(User *sender, std::string message);
};

#endif