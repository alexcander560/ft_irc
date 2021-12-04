#pragma once
#include "../../general.hpp"

// Структура для записи данных о пользователе (передаются командой USER при регистрации)
struct	userData
{
	string	_username;
	string	_hostname;
	string	_servername;
	string	_realname;
	time_t	_timeAfterPing;
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

	public:
		// Конструктор
		User(int id);
		// Деструктор
		~User();

		// Возвращает имя пользователя
		const string	getName() const;
		// Возвращает статус пользователя
		const int		getStatus() const;
		const string	getUserName() const;
		const string	getHostName() const;
		const string	getServerName() const;
		const string	getRealName() const;
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
		friend bool			operator == (const User &first, const User &second);
};
