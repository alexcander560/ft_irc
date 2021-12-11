#pragma once
#include "../../general.hpp"
#include "../messegeHandler/additions.hpp"

class Channel
{	
	private:
		string	name;		// имя канала
		string	topic;		// имя канала
	public:
		// Набор пользователей в канале, порядок вхождения на канал
		map<int, bool>	user;
		// Конструктор
		Channel (string name, int id) {
			if (setName(name) == false)
				throw (-1);
			topic = "No topic is set";
			user.insert(make_pair(id, true));
		}
		// Вовзвращает кол-во юзеров в канале
		int				getCountUSer() { return user.size(); }
		// Получить Id опера
		int				getIdOper() {
			for (map<int, bool>::iterator it = user.begin(); it != user.end(); it++)
				if (it->second)
					return it->first;
			return -1;
		}
		// Получить массив юзеров
		map<int, bool>	getUserList() { return user; }
		// Добавить Юзера
		bool			addUser(int	id, string pass = "") {
			(void)pass;
			if ((user.insert(make_pair(id, false))).second)
				debug(GREEN"[addUser] Вы успешно присоединились"DEFAULT);
			else {
				debug(RED"[addUser] Вы уже находитесь в канале"DEFAULT);
				return (false);
			}
			return (true);
		}
		// Удалить Юзера
		bool		delUser(int	id) {
			int		size_start, size_end;
			bool	flag = false;
			int		old_size = user.size(), size_new;
			bool	res = false;

			debug(GREEN"[addUser] Пытаюсь удалить юзера..."DEFAULT);
			size_start = user.size();
			size_end = user.size();
			if (user.find(id) != user.end()) {
				if (user.find(id)->second == true) {
					debug(GREEN"[addUser] Оператор будет удалён..."DEFAULT);
					flag = true;
				}
			}
			user.erase(id);
			size_new = user.size();
			if (old_size != size_new)
				res = true;
			if (flag == true)
				user.begin()->second = true;
			return (res);
		}
		// Установить имя канала
		bool			setName(const string name) {
			int	len = name.size();

			if (len < 2 || len >= 200) {
				debug(RED"[setName] Имя канала слишком короткое или слишком длинное"DEFAULT);
				return (false);
			}
			else if (name[0] != '&' && name[0] != '#') {
				debug(RED"[setName] Имя канала должно начинаться со спец символа (& или #)"DEFAULT);
				return (false);
			}
			else {
				if (name.find('\r') != std::string::npos || name.find('\n') != std::string::npos || name.find(' ') != std::string::npos|| name.find('\0') != std::string::npos || name.find(',') != std::string::npos) {
					debug(RED"[setName] В имени канала есть запрещённый символ"DEFAULT);
					return (false);
				}
			}
			debug(GREEN"[setName] Имя канала успешно установлено"DEFAULT);
			this->name = name;
			return (true);
		}
		// Возвращает имя канала
		void			printChannel() {
			cout
			<< "----------------------------------------" << endl
			<< "NAME: " << name << endl
			<< "TOPIC: " << topic << endls
			<< "ID USER(FLAG OPER) " << endl;
			for (map<int, bool>::iterator i = user.begin() ; i != user.end(); i++)
				cout << i->first << " (" << i->second << ")" << endl;
			cout << "----------------------------------------" << endl;
		}
		// Возвращает имя канала
		const string	getName() const { return name; }
		void			setTopic(string topic) { this->topic = topic; }
		// Возвращает топик канала
		const string	getTopic() const { return topic; }
		void	debug(string line)
		{
			if (DEBUG)
				cout << "DEBUG: " << line << endl;
		}
};