#include "User.hpp"

User::User(int id)
{
	_nick.second = false;
	_pass = false;
	_data.second = false;
	_status = -1;
	_id = id;
	debug("Новый пользователь создан");
}

User::~User()
{
	debug("Пользователь удалён");
}

const string	User::getName() const { return (_nick.first); }

bool			User::setNick(const string name)
{
	int		len = name.size(), count = 0;
	bool	flag = false;

	//cout << "(" << name << ")" << endl;
	while (name[count] && name[count] == ' ')
		count++;
	len -= count;
	if (len > 9 || len <= 0)
	{
		cout << "Неверная длина ника\n";
		return (false);
	}
	for (int i = count; i < len; i++)
	{
		if (!isalnum(name[i]) && name[i] != '-' && name[i] != '[' && name[i] != ']' &&
			name[i] != '\\' && name[i] != '^' && name[i] != '{' && name[i] != '}')
			return (false);
	}
	_nick.first = name.substr(count, len);
	_nick.second = true;
	return (_nick.second);
}

bool			User::setPass(const string pass_user, const string pass_server)
{
	int	count = 0;

	while (pass_user[count] && pass_user[count] == ' ')
		count++;
	cout << "pass_user=(" << pass_user << ")" << "pass_server=(" << pass_server << ")\n";
	pass_user.substr(count, pass_user.size()) == pass_server ? _pass = true : _pass = false;
	return (_pass);
}

bool			User::setData(const string data)
{
	int		len = data.size(), start = 0, count = 0;
	bool	flag = false;
	string	username, hostname, servername, realname;

	cout << "(" << data << ")" << endl;

	while (data[start] && data[start] == ' ')
		start++;

	username = data.substr(start, data.find(' ', start) - start);

	start += username.size();
	while (data[start] && data[start] == ' ')
		start++;
	hostname = data.substr(start, data.find(' ', start) - start);
	start += hostname.size();
	while (data[start] && data[start] == ' ')
		start++;
	servername = data.substr(start, data.find(' ', start) - start);
	start += servername.size();
	while (data[start] && data[start] == ' ')
		start++;
	if (data[start] == ':')
	{
		realname = data.substr(start + 1, data.size());
		start++;
	}
	else
		realname = data.substr(start, data.find(' ', start) - start);
	start += realname.size();

	for (;data[start]; start++)
	{
		if (data[start] != ' ')
			return (false);
	}

	if (username != "" && hostname != "" && servername != "" && realname != "")
	{
		_data.first._username = username;
		_data.first._hostname = hostname;
		_data.first._servername = servername;
		_data.first._realname = realname;
		_data.second = true;
	}
	return (_data.second);
}

void			User::printUser() const
{
	cout << "----------\n";
	cout << "USER ID: " << _id << endl;
	cout << "USER STATUS: " << _status << endl;
	cout << "pass: ";
	_pass ? cout << "Установлен\n" : cout << "Не установлен\n";
	cout << "nick: ";
	_nick.second ? cout << _nick.first << endl : cout << "Не установленно\n";
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
	cout << "----------\n";
}
