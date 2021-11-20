#include "group.hpp"

size_t	getCurrentTime(void)
{
	size_t			time;
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (time);
}

std::pair<t_groupData, User> &getCurrentOperator(std::vector< std::pair<t_groupData, User> > &users)
{
	std::vector< std::pair<t_groupData, User> >::iterator begin = users.begin(), end = users.end();

	if (users.size() == 0)
		throw (std::string("Current operator not found"));
	while (begin != end) {
		if (begin->first.is_operator)
			return (*begin);
		begin++;
	}
	throw (std::string("Current operator not found"));
}

bool	isUserOperator(std::vector< std::pair<t_groupData, User> > &users, User user)
{
	std::pair<t_groupData, User> currentOperator = getCurrentOperator(users);

	if (currentOperator.second == user)
		return (true);
	return (false);
}

void	setAnotherOperator(std::vector< std::pair<t_groupData, User> > &users)
{
	std::vector< std::pair<t_groupData, User> >::iterator *nearestOperator = NULL;
	try
	{
		std::pair<t_groupData, User> currentOperator = getCurrentOperator(users);
		std::vector< std::pair<t_groupData, User> >::iterator begin = users.begin();
		std::vector< std::pair<t_groupData, User> >::iterator end = users.end();
		while (begin != end)
		{
			if (begin->second == currentOperator.second)
				goto next;
			if (nearestOperator != NULL && (*nearestOperator)->first.join_time > begin->first.join_time)
				nearestOperator = &begin;
			else if (nearestOperator == NULL)
				nearestOperator = &begin;
		next:
			begin++;
		}
		(*nearestOperator)->first.is_operator = true;
	}
	catch (std::string &e)
	{
		throw (e);
	}
}
