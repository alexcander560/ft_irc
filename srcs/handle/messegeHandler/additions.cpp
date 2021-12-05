#include "additions.hpp"

const std::string	getCurrentTime()
{
	time_t	now_time;
	time(&now_time);
	return (std::string(std::ctime(&now_time)));
}

std::map<int, User>::iterator getUserByName(std::map<int, User> *clients, std::string name)
{
	std::map<int, User>::iterator begin = clients->begin();
	std::map<int, User>::iterator end = clients->end();

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
