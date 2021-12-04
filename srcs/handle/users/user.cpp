#include "user.hpp"

// Конструктор
User::User(int id, string ip)
{
	_nick.second = false;
	_pass = false;
	_data.second = false;
	_status = -1;
	_id = id;
	_data.first._timeAfterPing = time(0);
	_ip = ip;
}

// Деструктор
User::~User(){}

// Возвращает имя пользователя
const string	User::getName() const { return (_nick.first); }

// Возвращает статус пользователя
const int		User::getStatus() const { return (_status); }

const string	User::getUserName() const { return (_data.first._username); }

const string	User::getHostName() const { return (_data.first._hostname); }

const string	User::getRealName() const { return (_data.first._realname); }

const string	User::getServerName() const { return (_data.first._servername); }

const string 	User::getIp() const { return (_ip); }

// Устанавливает пользователю имя (проверяет на валидность)
// Возвращает true, если ник успешно установлен, fasle если ник не валиден
bool			User::setNick(vector<string> param)
{
	int	len = param[1].size();

	if (param[1] == "bot")
	{
		debug(RED"[setNick] Имя только для бота" DEFAULT);
		return (false);
	}
	if (len > 9 || len <= 0)
	{
		debug(RED"[setNick] Неверная длина nick" DEFAULT);
		return (false);
	}
	for (int i = 0; i < len; i++)
	{
		if (!isalnum(param[1][i]) && param[1][i] != '-' && param[1][i] != '[' && param[1][i] != ']' &&
		param[1][i] != '\\' && param[1][i] != '^' && param[1][i] != '{' && param[1][i] != '}')
		{
			debug(string(RED "[setNick] Запрещённый символ (")+ param[1][i] + ") в имени" DEFAULT);
			return (false);
		}
	}
	_nick.first = param[1];
	_nick.second = true;
	debug("[setNick] Ник успешно установлен");
	return (true);
}

// Проверяет ввёл ли пользователь верный пароль
// Возвращет true, если пароль верный, false если пароль неправильный
bool			User::setPass(vector<string> param, const string pass_server)
{
	debug("[setPass] Пароль сервера: (" + pass_server + ") Пароль клиента: (" + param[1] + ")");
	if (param.size() == 2)
	{
		if (param[1] == pass_server)
			_pass = true;
		else
			_pass = false;
	}
	else
	{
		_pass = false;
		debug(RED"[setPass] Неверное число аргументов" DEFAULT);
	}
	debug(_pass ? "[setPass] Введён верный пароль" : RED"[setPass] ПАРОЛЬ НЕВЕРНЫЙ" DEFAULT);
	return (_pass);
}

// Устанавливает данные пользователя
// Возвращает true, если данные успешно установлены, fasle при провале
bool			User::setData(vector<string> param)
{
	if (param.size() == 5)
	{
		_data.first._username = param[1];
		_data.first._hostname = param[2];
		_data.first._servername = param[3];
		_data.first._realname = param[4];
		_data.second = true;
	}
	else
	{
		debug(RED"[setData] Неверное число парметров" DEFAULT);
		return (false);
	}

	return (true);
}

// Проверяет все ли данные для пользователя заполненны, если да, то статус меняется на 1
bool			User::registration()
{
	int old_status = _status;

	if (_nick.second && _pass && _data.second)
		_status = 1;
	return (_status == 1 && old_status == -1 ? true : false);
}

// Распечатать все данные о пользователе
void			User::printUser() const
{
	cout << "--------------------------------------------------\n";
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
	cout << "ip: " << _ip << endl;
	cout << "--------------------------------------------------\n";
}

bool			operator == (const User &first, const User &second) // TODO: ALEX
{
	return (first.getName() == second.getName());
}