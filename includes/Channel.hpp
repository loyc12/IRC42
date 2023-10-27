#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "IRC.hpp"

# define CONSTR_CHAN 	"0======= PARAM-CONSTR(CHANNEL) ======0\n"
# define DEST_CHAN 		"0======== DESTRUCT-(CHANNEL) ========0\n"

class Channel
{
	private:
		std::string _chanName;
		std::string _adminName;
		std::vector<User*> _chanMembers;
		std::vector<User*> _chanOps;
		std::string	_topic;

		bool		_isInviteOnly;
		bool		_canTopic;
		bool		_keyFlag;
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
		bool		const &getInviteFlag 		(void) const;
		bool		const &getTopicFlag			(void) const;
		bool		const &getKeyFlag			(void) const;
		int				   getMemberCnt			(void) const;
		int 			   getOpCnt				(void) const;

		void	setChanName						(std::string const &chan);
		void	setAdminName					(std::string const &admin);
		void	setPass							(std::string const &password);
		void	setTopic						(std::string const &topic);
		void	setMaxMemberCount				(int const &count);
		void	setInviteFlag					(bool const &boolean);
		void	setTopicFlag					(bool const &boolean);
		void	setKeyFlag						(bool const &boolean);

		bool	isSameUser						(User *user1, User *user2);
		bool	isChanOp						(User *user);
		bool	hasSameNick						(User *user);

		bool	hasMember						(User *user);
		bool	hasChanOp						(User *user);

		void	addMember						(User *user);
		void	addChanOp						(User *user);

		void	removeMember					(User *user);
		void	removeChanOp					(User *user);

		void	resetOpp						(void);

		User 	*getMember						(int i);
		void	updateMemberList				(User *user);

// 		OVERLOAD
		void	sendToChan						(User *sender, std::string message, bool sendToSender);
};

#endif
