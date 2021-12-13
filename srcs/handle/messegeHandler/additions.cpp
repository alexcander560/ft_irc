#include "additions.hpp"
#include <sstream>

string	getCurrentTime() {
	time_t	now_time;
	time(&now_time);
	return (string(ctime(&now_time)));
}

time_t		getCurrentTimeForUser() {
	time_t	now_time;
	time(&now_time);
	return (now_time);
}

map<int, User>::iterator getUserByName(map<int, User> *clients, string name) {
	map<int, User>::iterator begin = clients->begin(), end = clients->end();

	while (begin != end) {
		if (begin->second.getName() == name)
			return (begin);
		begin++;
	}
	throw (0);
	return (begin);
}

std::string	int_to_string(size_t number) {
	std::ostringstream stream_for_convert;
	stream_for_convert << number;
	return (stream_for_convert.str());
}
