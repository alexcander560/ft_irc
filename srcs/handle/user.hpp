#include "../../general.hpp"

// Структура для записи данных о пользователе (передаются командой USER при регистрации)
struct userData
{
	string	_username;
	string	_hostname;
	string	_servername;
	string	_realname;
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
		int						_status;

	public:
		User();
		~User();

		// Устанавливает пользователю имя (проверяет на валидность)
		bool	setNick(const string name);
		// Проверяет ввёл ли пользователь верный пароль
		bool	setPass(const string pass_user, const string pass_server);
		// Устанавливает пользователю все нужные данные (проверяет на валидность)
		bool	setData(const string data);
		// Проверяет все ли данные для пользователя заполненны
		bool	dataAll();	
		// Распечатать все данные о пользователе
		void	printUser() const;
};
