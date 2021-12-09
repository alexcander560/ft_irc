#pragma once
#include "../../general.hpp"
#include "../messegeHandler/additions.hpp"

class Channel
{	
	// По хоршему надо что бы все переменные были приватными, но пофигу
	public:
		string	name;		// имя канала
		bool	flag_o;		// брать/давать привилегии операторов канала
		bool	flag_p;		// флаг приватности канала;
		bool	flag_s;		// флаг секретности канала
		bool	flag_i;		// флаг канала invite-only
		bool 	flag_t;		// при установке этого флага, менять топик могут только операторы
		bool	flag_n;		// запрещает сообщения на канал от посторонних клиентов
		//bool	flag_m;		// модерируемый канал
		bool	flag_l;		// установка ограничения на количество пользователей
		bool	flag_b;		// установка маски бана
		string	ban_mak;	// маска бана
		//bool	flag_v;		// брать/давать возможность голоса при модерируемом режиме
		bool	flag_k;		// установка пароля на канал
		string	pass;		// пароль канала
		// Набор пользователей в канале, является ли пользователь оператором
		vector<pair<int, bool> > user;

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
				if (name.find('\r') || name.find('\n') || name.find(' ') || name.find('\0') || name.find(',')) {
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
			<< "o(" << flag_o << ")" << endl
			<< "p(" << flag_p << ")" << endl
			<< "s(" << flag_s << ")" << endl
			<< "i(" << flag_i << ")" << endl
			<< "t(" << flag_t << ")" << endl
			<< "n(" << flag_n << ")" << endl
			//<< "m(" << flag_m << ")" << endl
			<< "l(" << flag_t << ")" << endl
			<< "b(" << flag_b << ")" << " {" << ban_mak << "}" << endl
			//<< "v(" << flag_v << ")" << endl
			<< "k(" << flag_k << ")" << " {" << pass << "}" << endl;
			for (int i = 0; i < user.size(); i++)
				cout << user[i].first << " (" << user[i].second << ")" << endl;
			cout << "----------------------------------------" << endl;
		}
		void	debug(string line)
		{
			if (DEBUG)
				cout << "DEBUG: " << line << endl;
		}
};