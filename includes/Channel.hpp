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
		std::vector<User*> _chanMembers; //
		std::vector<User*> _chanOps;
		std::string	_topic;

		bool		_isInviteOnly;
		bool		_canTopic;//Chan
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
		bool		const &getTopicFlag			(void) const;

		void	setChanName						(std::string const &chan);
		void	setAdminName					(std::string const &admin);
		void	setPass							(std::string const &password);
		void	setTopic						(std::string const &topic);
		void	setMaxMemberCount				(int const &count);
		void	setInviteFlag					(bool const &boolean);
		void	setTopicFlag					(bool const &boolean);

		bool	isSameUser						(User *user1, User *user2);
		bool	isChanOp						(User *user);
		bool	hasSameNick						(User *user);
//		MEMBER
		bool	hasMember						(User *user);
		void	addMember						(User *user);
		void	removeMember					(User *user);
		User 	*getMember						(int i);
		void	updateMemberList				(User *user);
		void	printMembers					(void);

//		CHANOP
		bool	hasChanOp						(User *user);
		void	addChanOp						(User *user);
		void	removeChanOp					(User *user);

//		User 	*getChanOp						(int i);
//		void	updateChanOpList				(User *user);
//		void	printChanOp						(void);


//		void	replyToChan						(User *user, std::string code, std::string input);

// 		OVERLOAD
		void	sendToChan						(User *sender, std::string message, bool sendToSender);
};

#endif
