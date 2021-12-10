#include "ping.hpp"
#include "../handle/messegeHandler/additions.hpp"

#define PING_SECOND 60

void	ping_client(std::map<int, User> &users, const int &id)
{
	std::vector< std::pair<int, std::string> >	messages;

	//users.find(id)->second.
	messages.push_back(make_pair(id, "PING"));
	send_message(messages);
}

void	check_time(std::map<int, User> &users)
{
	std::map<int, User>::iterator begin = users.begin();
	std::map<int, User>::iterator end = users.end();
	t_time	current_time = getCurrentTimeForUser();
	double	different;

	while (begin != end)
	{
		different = difftime(current_time, begin->second.getTimePing());
		if (different > PING_SECOND)
			ping_client(users, begin->first);
	}
}
