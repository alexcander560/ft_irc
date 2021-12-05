#pragma once
#include "../../general.hpp"

// Структура для записи данных о пользователе (передаются командой USER при регистрации)
struct	userData
{
	string	username;
	string	hostname;
	string	servername;
	string	realname;
	time_t	timeAfterPing;
};

struct	userMode
{
	bool	i;
	bool	s;
	bool	o;
	bool	w;
};

// Класс содержит всё информацию о пользователе
// Первый элемент пары: данные о пользователя
// Второй элемент пары: наличии данных (0 - нет, 1 - да)
class User
{
	private:
		pair<string, bool>		_nick;
		bool					_pass;
		pair<userData, bool>	_data;
		int						_status;	// '-1' - не прошёл регистрацию, '1' - зарегестрирован
		int						_id;		// Излишние данные; для удобства печати информации о пользователе
		string 					_ip;
		pair<bool, string>		_away_message; //Сообщение, устанавливаемое командой AWAY {MESSAGE} - служит для автоматического ответа при PRIVMSG к юзеру.
		userMode				_mode;

	public:
		// Конструктор
		User(int id, string ip);
		// Деструктор
		~User();

		// Возвращает имя пользователя
		const string				getName() const;
		// Возвращает статус пользователя
		const int					getStatus() const;
		const string				getUserName() const;
		const string				getHostName() const;
		const string				getServerName() const;
		const string				getRealName() const;
		const string				getIp() const;
		const userMode				getMode() const;
		const pair<bool, string>	getAwayMessage() const;
		time_t						getTime() const;
		void						setAwayMessage(pair<bool, string> away_message);
		void						setModeI(bool flag);
		void						setModeS(bool flag);
		void						setModeO(bool flag);
		void						setModeW(bool flag);
		void						setTime(time_t time);
		// Устанавливает пользователю имя (проверяет на валидность)
		bool			setNick(vector<string> param);
		// Проверяет ввёл ли пользователь верный пароль
		bool			setPass(vector<string> param, const string pass_server);
		// Устанавливает пользователю все нужные данные (проверяет на валидность)
		bool			setData(vector<string> param);
		// Проверяет все ли данные для пользователя заполненны, если да, то статус меняется на 1
		bool			registration();
		// Распечатать все данные о пользователе
		void			printUser() const;
		//Оператор сравнения двух пользователей
		friend bool		operator == (const User &first, const User &second);
};
