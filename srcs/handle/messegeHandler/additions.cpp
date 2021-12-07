#include "additions.hpp"

const string	getCurrentTime()
{
	time_t	now_time;
	time(&now_time);
	return (string(ctime(&now_time)));
}

const time_t		getCurrentTimeForUser()
{
	time_t	now_time;
	time(&now_time);
	return (now_time);
}

map<int, User>::iterator getUserByName(map<int, User> *clients, string name)
{
	map<int, User>::iterator begin = clients->begin();
	map<int, User>::iterator end = clients->end();

	while (begin != end)
	{
		if (begin->second.getName() == name)
			goto done;
		begin++;
	}
	throw (0);
done:
	return (begin);
}
