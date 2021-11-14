#include "user.hpp"

User::User()
{
	_status = -1;
	if (DEBUG)
		debug("[User()] Новый пользователь создан");
}

User::~User()
{
	if (DEBUG)
		debug("[~User()] Пользователь удалён");
}

bool	User::setNick(const string name)
{
	size_t len = name.size();

	if (len > 9 || len <= 0)
		return (0);

	for (size_t i = 0; i < len; i++)
	{
		if (!(isalpha(name[i]) || isalnum(name[i]) || name[i] == '-' ||
			name[i] == '[' || name[i] == ']' || name[i] == '\\' ||
			name[i] == '^' || name[i] == '{' || name[i] == '}'))
			return (false);
	}

	_nick.first = name;
	_nick.second = true;
	return (true);
}

bool	User::setPass(const string pass_user, const string pass_server)
{
	return (pass_user == pass_server ? _pass = true : _pass = false);
}

bool	User::setData(const string data)
{
	size_t	len = data.size(), start = 0, len_substr = 0, count = 0;
	bool	flag = false;

	cout << data << endl;

	for (size_t i = 0; i < len; i++)
	{
		if (data[i] == ' ')
			start++;
		else if (data[i] == ':' && count == 3)
		{
			_data.first._realname = data.substr(start + 1, len);
			break ;
		}
		else if (data[i] != ' ')
		{
			for (; i < len; i++)
			{
				if (data[i] == ' ' || (i + 1) == len)
				{
					if (count == 0)
						_data.first._username = data.substr(start, data[i] == ' ' ? i - start : i - start + 1);
					else if (count == 1)
						_data.first._hostname = data.substr(start, data[i] == ' ' ? i - start : i - start + 1);
					else if (count == 2)
						_data.first._servername = data.substr(start, data[i] == ' ' ? i - start : i - start + 1);
					else if (count == 3)
						_data.first._realname = data.substr(start, data[i] == ' ' ? i - start : i - start + 1);
					else
						return (false);
					count++;
					start = i + 1;
					break ;
				}
			}
		}
	}
	_data.second = true;
	return (true);
}

void	User::printUser() const
{
	//cout << "USER PRINT" << endl;
	cout << "USER STATUS: " << _status << endl;
	cout << "pass: ";
	if (_pass)
		cout << "Установлен" << endl;
	else
		cout << "Не установлен" << endl;
	cout << "nick: ";
	if (_nick.second)
		cout << _nick.first << endl;
	else
		cout << "Не установленно" << endl;
	cout << "user: ";
	if (_data.second)
	{
		cout
		<< "username:   [" << _data.first._username << "]" << endl
		<< "      hostname:   [" << _data.first._hostname << "]" << endl
		<< "      servername: [" << _data.first._servername << "]" << endl
		<< "      realname:   [" << _data.first._realname << "]" << endl;
	}
	else
		cout << "Не установленно" << endl;

}
