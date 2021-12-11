#pragma once
#include "../../general.hpp"
#include "../messegeHandler/additions.hpp"

class Channel
{	
	private:
		string	name;		// имя канала
		string	topic;		// имя канала
	// По хоршему надо что бы все переменные были приватными, но пофигу
	public:
		//bool			flag_o;		// брать/давать привилегии операторов канала
		//bool			flag_p;		// флаг приватности канала;
		//bool			flag_s;		// флаг секретности канала
		//bool			flag_i;		// флаг канала invite-only
		//bool			flag_t;		// при установке этого флага, менять топик могут только операторы
		//bool			flag_n;		// запрещает сообщения на канал от посторонних клиентов
		//bool			flag_m;		// модерируемый канал
		//bool			flag_l;		// установка ограничения на количество пользователей
		//bool			flag_b;		// установка маски бана
		//set <string>	ban_masks;	// маска бана
		//bool			flag_v;		// брать/давать возможность голоса при модерируемом режиме
		//bool			flag_k;		// установка пароля на канал
		//string			pass;		// пароль канала
		// Набор пользователей в канале, порядок вхождения на канал
		map<int, bool>	user;
		// Конструктор
		Channel (string name, int id) {
			if (setName(name) == false)
				throw (-1);
			//flag_o = false;
			//flag_p = false;
			//flag_s = false;
			//flag_i = false;
			//flag_t = false;
			//flag_n = false;
			//flag_m = false;
			//flag_l = false;
			//flag_k = false;
			//pass = "";
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
			//if (flag_k == false) {
				if ((user.insert(make_pair(id, false))).second)
					debug(GREEN"[addUser] Вы успешно присоединились"DEFAULT);
				else {
					debug(RED"[addUser] Вы уже находитесь в канале"DEFAULT);
					return (false);
				}
			//}
			// else if (pass == this->pass) {
			// 	user.insert(make_pair(id, false));
			// 	debug(GREEN"[addUser] Вы успешно присоединились, был введён верный пароль"DEFAULT);
			// }
			// else {
			// 	debug(GREEN"[addUser] В доступе отказано, пароль неверный"DEFAULT);
			// 	return (false);
			// }
			return (true);
		}
		// Удалить Юзера
		void			delUser(int	id) {
			int		size_start, size_end;
			bool	flag = false;

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
			if (flag == true)
				user.begin()->second = true;
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
			<< "TOPIC: " << topic << endl
			//<< "o(" << flag_o << ")" << endl
			//<< "p(" << flag_p << ")" << endl
			//<< "s(" << flag_s << ")" << endl
			//<< "i(" << flag_i << ")" << endl
			//<< "t(" << flag_t << ")" << endl
			//<< "n(" << flag_n << ")" << endl
			//<< "m(" << flag_m << ")" << endl
			//<< "l(" << flag_t << ")" << endl
			//<< "v(" << flag_v << ")" << endl
			//<< "k(" << flag_k << ")" << " {" << pass << "}" << endl
			<< "ID USER(FLAG OPER) " << endl;
			for (map<int, bool>::iterator i = user.begin() ; i != user.end(); i++)
				cout << i->first << " (" << i->second << ")" << endl;
			// cout << "Всего масок бана: " << ban_masks.size() << endl;
			// for (set<string>::iterator i = ban_masks.begin(); i != ban_masks.end(); i++)
			// 	cout << *i << endl;
			cout << "----------------------------------------" << endl;
		}
		// Возвращает имя канала
		const string	getName() const { return name; }
		void			setTopic(string topic) { this->topic = topic; }
		// Возвращает топик канала
		const string	getTopic() const { return topic; }
		// Cвой дебаг, если у тебя есть вопросы что он тут делает, то я могу сказать одно
		// это тебя волновать не должно, ещё вопросы?
		void	debug(string line)
		{
			if (DEBUG)
				cout << "DEBUG: " << line << endl;
		}
};