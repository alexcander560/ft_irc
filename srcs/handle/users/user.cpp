#include "user.hpp"
#include "../messegeHandler/additions.hpp"

// Конструктор
User::User(int id, string ip)
{
	_nick.second = false;
	_pass = false;
	_data.second = false;
	_status = -1;
	_id = id;
	_data.first.timeAfterPing = time(0);
	_data.first.timeStart = getCurrentTimeForUser();
	_data.first.timeIdle = time(0);
	_ip = ip;
	_away_message = make_pair(false, "");
	_mode.i = false;
	_mode.s = false;
	_mode.o = false;
	_mode.w = false;
}

// Деструктор
User::~User(){}

// Все get
const string				User::getName() const			{ return (_nick.first); }
const int					User::getStatus() const			{ return (_status); }
const string				User::getUserName() const		{ return (_data.first.username); }
const string				User::getHostName() const		{ return (_data.first.hostname); }
const string				User::getRealName() const		{ return (_data.first.realname); }
const string				User::getServerName() const		{ return (_data.first.servername); }
const string				User::getIp() const				{ return (_ip); }
const userMode				User::getMode() const			{ return (_mode); }
const pair<bool, string>	User::getAwayMessage() const	{ return (_away_message); }
time_t						User::getTimePing() const		{ return (_data.first.timeAfterPing); }
time_t						User::getTimeIdle() const		{ return (_data.first.timeIdle); }
// Все set
void						User::setAwayMessage(pair<bool, string> away_message)	{ _away_message = away_message; }
void						User::setModeI(bool flag)		{ _mode.i = flag; }
void						User::setModeS(bool flag)		{ _mode.s = flag; }
void						User::setModeO(bool flag)		{ _mode.o = flag; }
void						User::setModeW(bool flag)		{ _mode.w = flag; }
void						User::setTimePing(time_t time)	{ _data.first.timeAfterPing = time; }
void						User::setTimeIdle(time_t time)	{ _data.first.timeIdle = time - _data.first.timeStart; }

// Устанавливает пользователю имя (проверяет на валидность)
// Возвращает true, если ник успешно установлен, fasle если ник не валиден
bool			User::setNick(vector<string> param) {
	int	len = param[1].size();

	if (param[1] == "bot") {
		debug(RED"[setNick] Имя только для бота"DEFAULT);
		return (false);
	}
	if (len > 9 || len <= 0) {
		debug(RED"[setNick] Неверная длина nick"DEFAULT);
		return (false);
	}
	for (int i = 0; i < len; i++) {
		if (!isalnum(param[1][i]) && param[1][i] != '-' && param[1][i] != '[' && param[1][i] != ']' &&
		param[1][i] != '\\' && param[1][i] != '^' && param[1][i] != '{' && param[1][i] != '}') {
			debug(string(RED"[setNick] Запрещённый символ (")+ param[1][i] + ") в имени"DEFAULT);
			return (false);
		}
	}
	_nick.first = param[1];
	_nick.second = true;
	debug(GREEN"[setNick] Ник успешно установлен"DEFAULT);
	return (true);
}

// Проверяет ввёл ли пользователь верный пароль
// Возвращет true, если пароль верный, false если пароль неправильный
bool			User::setPass(vector<string> param, const string pass_server)
{
	debug("[setPass] Пароль сервера: (" + pass_server + ") Пароль клиента: (" + param[1] + ")");
	if (param.size() == 2) {
		if (param[1] == pass_server)
			_pass = true;
		else
			_pass = false;
	}
	else {
		_pass = false;
		debug(RED"[setPass] Неверное число аргументов" DEFAULT);
	}
	debug(_pass ? GREEN"[setPass] Введён верный пароль"DEFAULT : RED"[setPass] ПАРОЛЬ НЕВЕРНЫЙ" DEFAULT);
	return (_pass);
}

// Устанавливает данные пользователя
// Возвращает true, если данные успешно установлены, fasle при провале
bool			User::setData(vector<string> param)
{
	if (param.size() == 5) {
		_data.first.username = param[1];
		_data.first.hostname = param[2];
		_data.first.servername = param[3];
		_data.first.realname = param[4];
		_data.second = true;
	}
	else {
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
	cout << "AWAY: " << _away_message.second << "(" << _away_message.first << ")" << endl;
	cout << "pass: ";
	_pass ? cout << "Установлен\n" : cout << "Не установлен\n";
	cout << "nick: ";
	_nick.second ? cout << _nick.first << endl : cout << "Не установленно\n";
	cout << "user: ";
	if (_data.second) {
		cout
		<< "username:   [" << _data.first.username << "]" << endl
		<< "      hostname:   [" << _data.first.hostname << "]" << endl
		<< "      servername: [" << _data.first.servername << "]" << endl
		<< "      realname:   [" << _data.first.realname << "]" << endl;
	}
	else
		cout << "Не установленно" << endl;
	cout << "  ip: " << _ip << endl;
	cout << "timePING: " << _data.first.timeAfterPing << endl;
	cout << "timeIDLE: " << _data.first.timeIdle << endl;
	cout
	<< "mode: "
	<< "i(делает пользователя невидимым)              = " << _mode.i << endl
	<< "      s(пользователь (не)получает NOTICE сообщения) = " << _mode.s << endl
	<< "      o(флаг IRC-оператора)                         = " << _mode.o << endl
	<< "      w(пользователь (не)получает WALLOPS сообщения)= " << _mode.w << endl;
	cout << "--------------------------------------------------\n";
}

// Перегрузка оператора =
bool			operator == (const User &first, const User &second) { return (first.getName() == second.getName()); }
