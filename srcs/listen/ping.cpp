#include "ping.hpp"
#include "../handle/messegeHandler/additions.hpp"

#define PING_SECOND 10

#define PING_SECOND_KICK 15

void	ping_client(std::map<int, User> &users, const int &id)
{
	std::vector< std::pair<int, std::string> >	messages;

	users.find(id)->second.setIsPing(true); //TODO ALSO IN PONG
	messages.push_back(make_pair(id, "PING"));
	send_message(messages);
}

void	kick_client(std::map<int, User> &users, const int &id)
{
	std::vector< std::pair<int, std::string> >	messages;

	messages.push_back(make_pair(id, "QUIT"));
	send_message(messages);
}

void	check_time(std::map<int, User> &users)
{
	std::map<int, User>::iterator begin = users.begin();
	std::map<int, User>::iterator end = users.end();
	time_t	current_time = getCurrentTimeForUser();
	double	different;

	while (begin != end)
	{
		different = difftime(current_time, begin->second.getTimePing());
		if (different > PING_SECOND && !begin->second.getIsPing())
			ping_client(users, begin->first);
		else if (different > PING_SECOND_KICK)
		{
			kick_client(users, begin->first);
			std::cout << "User was kick" << std::endl; //TODO: KICK USER FOR TIMEOUT
		}
		begin++;
	}
}
