#include "additions.hpp"

const std::string	getCurrentTime()
{
	time_t	now_time;
	time(&now_time);
	return (std::string(std::ctime(&now_time)));
}
