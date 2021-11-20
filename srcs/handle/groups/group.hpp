#pragma once

#include <sys/time.h>
#include "../../general.hpp"

class User;

typedef struct s_groupData
{
	size_t	join_time;
	bool	is_operator;
}				t_groupData;

size_t	getCurrentTime(void);
std::pair<t_groupData, User> &getCurrentOperator(std::vector< std::pair<t_groupData, User> > &users);
void	setAnotherOperator(std::vector< std::pair<t_groupData, User> > &users);

class Group
{
private:
	string				channel_name;
	std::string			channel_topic;
	std::vector< std::pair<t_groupData, User> >	users;
public:
	Group(const string name);
	~Group();
	Group(const Group &another);
	Group	&operator = (const Group &another);
	size_t	add_user(User &user);
	size_t	delete_user(User &user);
	size_t	get_usersCount();
};
