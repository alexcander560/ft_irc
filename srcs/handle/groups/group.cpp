#include "group.hpp"

Group::Group(const string name)
{
	this->channel_name = name;
}

Group::~Group()
{
	std::vector< std::pair<t_groupData, User> >::iterator begin = this->users.begin();
	std::vector< std::pair<t_groupData, User> >::iterator end = this->users.end();

	while (begin != end)
	{
		this->users.erase(begin);
		begin++;
	}
	this->users.clear();
}

size_t Group::add_user(User &user)
{
	t_groupData	groupData;

	groupData.is_operator = false;
	if (this->get_usersCount() == 0)
		groupData.is_operator = true;
	groupData.join_time = getCurrentTime();
	std::pair<t_groupData, User>	dataUser(groupData, user);

	this->users.push_back(dataUser);
	return (get_usersCount());
}

size_t	Group::delete_user(User &user)
{
	std::vector< std::pair<t_groupData, User> >::iterator begin = this->users.begin();
	std::vector< std::pair<t_groupData, User> >::iterator end = this->users.end();

	while (begin != end)
	{
		if (begin->second == user)
		{
			this->users.erase(begin);
			return (this->get_usersCount());
		}
		begin++;
	}
	throw (std::string("User not found"));
}

size_t	Group::get_usersCount()
{
	return (this->users.size());
}
