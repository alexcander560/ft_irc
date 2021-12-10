#pragma once
#include "../../general.hpp"
#include "../../listen/support.hpp"
#include "additions.hpp"
#include "../general/answers.hpp"

//#include "../users/user.hpp"

class MassegeHandler {
private:
	// Создать строку для корректного вывода сообщения
	// Использовать с аргументом по умолчанию, когда хотим получить строку с данными ЭТОГО юзера. Подавать user_id в иных случаях
	const string	getFrontLine(int user_id = -1) const {
		if (user_id == -1)
			user_id = id;
		string	user = this->clients->find(user_id)->second.getName();
		string	name = this->clients->find(user_id)->second.getUserName();
		string	ipaddress = this->clients->find(user_id)->second.getIp();
		return (string(":") + user + string("!") + name + string("@") + ipaddress + string(" "));
	}
	// Создать строку для корректного вывода сообщения
	const string	getFrontLineRPL(string value, string rpl, int user_id = -1) {
		if (user_id == -1)
			user_id = id;
		string user = this->clients->find(user_id)->second.getName();
		return (":"SERVER_NAME" " + rpl + " " + user + " :" + value);
	}
	// Добавить сообщение на отправку
	void			add_message(int id, string str) {
		messages.push_back(make_pair(id, str));
	}
	// Для перебора массива с функциями
	typedef void (MassegeHandler::*Method) (pair<map<int, User>::iterator, bool> *);
	map<string, Method> commands;
	// Определяет подъодит ли строка под маску
	int		check_mask(string str, string mask_base) {
		bool	res = true, flag = false;
		string	mask = "";
		int		index = 0, len_str = str.size(), len_mask;

		if (mask_base.find('*') == -1)
			return (str == mask_base);
		for (int i = 0; mask_base[i]; i++) {
			if (mask_base[i] == '*' && i == 0)
				mask += mask_base[i];
			else if (mask_base[i] == '*' && mask_base[i - 1] != '*')
				mask += mask_base[i];
			else if (mask_base[i] != '*')
				mask += mask_base[i];
		}
		len_mask = mask.size();
		for (int i = 0; mask[i]; i++) {
			if (mask[i] == '*' && (i != len_mask - 1)) {
				flag = false;
				i++;
				while (index < len_str) {
					if (mask[i] == str[index]) {
						flag = true;
						index++;
						break ;
					}
					index++;
				}
				if (flag == false)
					return (false);
			}
			else if (mask[i] != '*') {
				if (mask[i] != str[index]) {
					return (false);
				}
				index++;
			}
		}
		return(res);
	}
	// Разбивает строку на набор параметров
	void	parser_param() {
		int		len = str_message.size(), end = len - 1;
		string	temp = "";
		bool	flag = false;

		// Удалим все пробелы в конце строки
		for (; end >= 0; end--)
			if (str_message[end] != ' ')
				break ;
		if (end != len - 1)
			str_message = str_message.substr(0, end + 1);
		len = str_message.size();
		// Цикл разбиения
		for (int i = 0; i < len; i++) {
			if (str_message[i] == ' ') {
				if (flag == true) {
					param.push_back(temp);
					temp = "";
					flag = false;
				}
				while (str_message[i] && str_message[i] == ' ')
					i++;
				i--;
			}
			else {
				if (str_message[i] == ':' && flag == false) {
					temp = str_message.substr(i + 1, len);
					break ;
				}
				flag = true;
				temp += str_message[i];
			}
		}
		if (temp != "")
			param.push_back(temp);
	}
	// Разбивает набор юзеров на массив юзеров
	bool	parser_set(string str, set<string> *set) {
		string	current_name("");
		int		i = 0;

		while (str[i]) {
			if (str[i] == ',') {
				if (current_name.size() > 0) {
					if (!(set->insert(current_name)).second)
						debug(RED"[parser_set] Двойной параметр");
					current_name.clear();
				}
			}
			else
				current_name.push_back(str[i]);
			i++;
		}
		if (current_name.size() > 0)
			if (!(set->insert(current_name)).second)
				debug(RED"[parser_set] Двойнок ник " + current_name + DEFAULT);
		return (true);
	}
	// Разбивает набор паролей на массив паролей
	bool	parser_vector(string str, vector<string> *vec) {
		string	current_name("");
		int		i = 0;

		while (str[i]) {
			if (str[i] == ',') {
				if (current_name.size() > 0) {
					vec->push_back(current_name);
					current_name.clear();
				}
			}
			else
				current_name.push_back(str[i]);
			i++;
		}
		if (current_name.size() > 0)
			vec->push_back(current_name);
		return (true);
	}	
	// Базовая проверка команда на верное кол-во аргументов и регистрацию пользователя
	// Если указан последний необязательный параметр, то функция будет проверять услвоие по-другому
	bool	command_base_check(int count_param, int status, bool flag = false) {
		if (lenparam != count_param && flag == false) {
			debug(RED"[command_base_check] " + param[0] + " неверное число аргументов"DEFAULT);
			return (false);
		}
		else if (lenparam == count_param && flag == true)
		{
			debug(RED"[command_base_check] " + param[0] + " слишком мало аргументов"DEFAULT);
			return (false);
		}
		if (status != 1) {
			debug(RED"[command_base_check] " + param[0] + " Нельзя выполнить до регистрации пользователя" DEFAULT);
			return (false);
		}
		return (true);
	}
	// Установка пароля
	void	command_pass(pair<map<int, User>::iterator, bool> *res) {
		if (res->first->second.getStatus() == -1)
			res->first->second.setPass(param, pass);
		else
			debug(RED"[handle_message] Вы уже зарегистрированы!" DEFAULT);
	}
	// Установка имени
	void	command_nick(pair<map<int, User>::iterator, bool> *res) {
		if (lenparam != 2)
			debug(RED"[handle_message] Неверное число аргументов для команды NICK"DEFAULT);
		else {
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++) {
				if (it1->second.getName() == param[1]) {
					debug(RED"[handle_message] Такой NICK уже есть"DEFAULT);
					return ;
				}
			}
			res->first->second.setNick(param);
		}
	}
	// Установка данных пользователя
	void	command_user(pair<map<int, User>::iterator, bool> *res) {
		res->first->second.setData(param);
	}
	// Прерывает соединение
	void	command_quit(pair<map<int, User>::iterator, bool> *res) {
		clients->erase(id);
		disconnect_by_id(this->id, this->clients_ivan, this->fds);
		debug(GREEN"[command_quit] Команда QUIT использовалась для пользователя"DEFAULT);
	}
	// Отправить сообщение (Пока не работает для каналов)
	void	command_privmsg(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(3, res->first->second.getStatus()))
			return ;
		if (param[1] == "bot") {
			debug(GREEN"[command_privmsg] Calling bot with command"DEFAULT);
			handle_command(param.at(2), id, clients->find(id)->second.getName(), &messages);
			return ;
		}
		parser_set(param[1], &user_list);
		for (set<string>::iterator us1 = user_list.begin(); us1 != user_list.end(); us1++){
			bool	flag = true;
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++) {
				if (it1->second.getName() == *us1) {
					if (it1->second.getStatus() != 1)
						debug(RED"[command_privmsg] Пользователь с таким ником не прошёл полную регистрацию"DEFAULT);
					else if (it1->second.getMode().s && param[0] == "NOTICE")
						debug(RED"[command_privmsg] У конечной точки стоит флаг +s. NOTICE не сработает!"DEFAULT);
					else
					{
						debug(GREEN"[command_privmsg] Пользователь найден, отправляю сообщение..."DEFAULT);
						add_message(it1->first, getFrontLine() + param[0] + " " + *us1 + " " + ((param[2][0] == ':') ? ("") : (":")) + param[2] + "\n");
						if (!is_notice && it1->second.getAwayMessage().first) {										/* Check AWAY - BEGIN */
							debug(GREEN"[command_privmsg] Away автоматическое сообщение было добавлено"DEFAULT);
							add_message(id, getFrontLine(it1->first) + param[0] + " " + clients->find(id)->second.getName() + " :" + it1->second.getAwayMessage().second + "\n");
						}
						else if (is_notice)
							debug("[command_privmsg] It's not need to send automessage, because it's notice");
						else
							debug("[command_privmsg] It's not need to send automessage");							/* Check AWAY - END */
					}
					flag = false;
					break ;
				}
			}
			if (flag)
				debug(RED"[handle_message] Пользователь с таким ником не найден" DEFAULT);
			//================== Каналы ====================================
			// СООБЩЕНИЕ НЕ ДОЛЖНО ОТПРАВЛЯТЬСЯ САМОМУ СЕБЕ
			flag = true;
			for (vector<Channel>::iterator it1 = channel->begin(); it1 != channel->end(); it1++) {
				if (it1->getName() == *us1) {
					debug(GREEN"[handle_message] Канал найден, отправляем всем сообщение..." DEFAULT);
					map<int, bool> temp_list = it1->getUserList();
					for (map<int, bool>::iterator temp_iter = temp_list.begin(); temp_iter != temp_list.end(); temp_iter++) {
						debug(GREEN"[handle_message] Отправляю сообщение..." DEFAULT);
						add_message(temp_iter->first, getFrontLine() + param[0] + " " + *us1 + " " + ((param[2][0] == ':') ? ("") : (":")) + param[2] + "\n");
					}
					flag = false;
					break ;
				}
			}
			if (flag)
				debug(RED"[handle_message] Канал с таким именем не найден" DEFAULT);
			//==============================================================
		}
	}
	// Устанавливает автоматический ответ на сообщение типа PRIVMSG
	void	command_away(pair<map<int, User>::iterator, bool> *res) {
		if (lenparam == 1) {
			debug("[command_away] Away message was unset");
			clients->find(id)->second.setAwayMessage(make_pair(false, ""));
		}
		else if (lenparam == 2) {
			debug("[command_away] Away message was set");
			clients->find(id)->second.setAwayMessage(make_pair(true, param[1]));
		}
		else
			warning(RED"[command_away] Неверное число аргументов"DEFAULT);
	}
	// PRIVMSG без добавления автоматического ответа при режиме +s
	void	command_notice(pair<map<int, User>::iterator, bool> *res) {
		is_notice = true;
		command_privmsg(res);
		is_notice = false;
	}
	// Настройка режима пользователя или каналя (Пока не работает для каналов)
	// Да собственно говоря эта функция работает совсем неправильно,
	// надо разбираться и исправлять
	void	command_mode(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(3, res->first->second.getStatus()))
			return ;
		if (param[1] == res->first->second.getName()) { // Вот эта потом нужно будет поменять, наверное
			if (param[2].size() != 2)
				debug(RED"[command_mode] Строка с параметрами неверной длины"DEFAULT);
			else if (param[2][0] != '+' && param[2][0] != '-')
				debug(RED"[command_mode] Знак mode неверный {" + string(1, param[1][0]) + "}"DEFAULT);
			else {
				if (param[2][1] == 'i')
					res->first->second.setModeI(param[2][0] == '+' ? true : false);
				else if (param[2][1] == 'w')
					res->first->second.setModeW(param[2][0] == '+' ? true : false);
				else if (param[2][1] == 's')
					res->first->second.setModeS(param[2][0] == '+' ? true : false);
				else if (param[2][1] == 'o')
				{
					if (param[2][0] == '-')
						res->first->second.setModeO(false);
					else
						debug(RED"[command_mode] нельзя получить права оператора таким способом"DEFAULT);
				}
				else
					debug(RED"[command_mode] флаг mode неверный {" + string(1, param[2][1]) + "}"DEFAULT);
			}
		}
		else
			debug(RED"[command_mode] Имя пользователя введено неверно"DEFAULT);
	}
	// Возвращает список пользователей в сети
	void	command_ison(pair<map<int, User>::iterator, bool> *res) {
		string						line;
		map<int, User>::iterator	iter;
		int							current = 1;
		short						length = 0;
		
		if (!command_base_check(1, res->first->second.getStatus(), true))
			return ;
		while (current < lenparam && length < 512) {

			bool flag = true;
			for (map<int, User>::iterator begin = clients->begin(); begin != clients->end(); begin++) {
				if (param[current] == begin->second.getName()) {
					flag = false;
					if (begin->second.getStatus() != 1)
						debug(RED"[command_ison] Пользователь с ником " + param[current] + " не прошёл регистрацию"DEFAULT);
					else {
						if (!line.empty())
							line.append(" ");
						line.append(param[current]);
					}
					break;
				}
			}
			if (flag)
				debug(RED"[command_ison] Пользователь с ником " + param[current] + " не найден"DEFAULT);
			current++;
			length += param[current].length();
		}
		add_message(id, getFrontLineRPL(line, RPL_ISON) + "\n");
	}
	// Возвращает информацию о пользователях (максимум 5)
	void	command_userhost(pair<map<int, User>::iterator, bool> *res) {
		string						line;
		map<int, User>::iterator	iter;
		int							current = 1;

		if (!command_base_check(1, res->first->second.getStatus(), true))
			return ;
		while (current < lenparam && current < 6){
			try
			{
				iter = getUserByName(clients, param[current]);
				if (!line.empty())
					line.append(" ");
				line.append(param[current] + "=" + ((iter->second.getMode().s) ? "-" : "+") + "@" + iter->second.getIp());
			}
			catch (int zero) {
				debug(RED"[command_userhost] Пользователь с ником " + param[current] + " не найден"DEFAULT);
			}
			current++;
		}
		add_message(id, getFrontLineRPL(line, RPL_USERHOST) + "\n");
	}
	// Возвращает информацию о версии сервера
	void	command_version(pair<map<int, User>::iterator, bool> *res) {
		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_version] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam > 2)
			debug(RED"[command_version] Неверное чсило аргументов "DEFAULT);
		else if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME))
			add_message(id, getFrontLineRPL(SERVER_VERSION, RPL_VERSION) + "\n");
		else
			debug(RED"[command_version] Имя сервера неверно"DEFAULT);
	}
	// Возвращает информацию о текущем сервере
	void	command_info(pair<map<int, User>::iterator, bool> *res) {
		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_info] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam > 2)
			debug(RED"[command_info] Неверное число аргументов"DEFAULT);
		else if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME)) {
			add_message(id, getFrontLineRPL("describe\n", RPL_INFO));
			add_message(id, getFrontLineRPL("IRC server based on TCP/IP protocol to rfc1459 standard\n", RPL_INFO));
			add_message(id, getFrontLineRPL("End of /INFO list\n", RPL_ENDOFINFO));
		}
		else
			debug(RED"[command_info] Неверное число аргументов"DEFAULT);
	}
	// Возвращает информацию об администраторе
	void	command_admin(pair<map<int, User>::iterator, bool> *res) {
		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_admin] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam > 2)
			debug(RED"[command_admin] Неверное число аргументов"DEFAULT);
		else if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME)) {
			add_message(id, getFrontLineRPL("", RPL_ADMINME) + "Information about administrators\n");
			add_message(id, getFrontLineRPL("", RPL_ADMINLOC1) + "Names are Giganta Mother, Irena Mora, Free Milissa\n");
			add_message(id, getFrontLineRPL("", RPL_ADMINLOC2) + "Usernames are gmother, imora, fmilissa\n");
			add_message(id, getFrontLineRPL("", RPL_ADMINEMAIL) + "Email are gmother@student.21-school.ru, imorastudent.21-school.ru, fmilissastudent.21-school.ru\n");
		}
		else
			debug(RED"[command_admin] Имя сервера неверно"DEFAULT);
	}
	// Возвращает локальное время
	void	command_time(pair<map<int, User>::iterator, bool> *res) {
		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_time] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam > 2)
			debug(RED"[command_time] Неверное число аргументов"DEFAULT);
		else if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME))
			add_message(id, getFrontLineRPL(getCurrentTime(), RPL_TIME));
		else
			debug(RED"[command_time] Имя сервера неверно"DEFAULT);
	}
	// Взятие операторских прав
	void	command_oper(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(3, res->first->second.getStatus()))
				return ;
		if (param[1] != res->first->second.getName()) {// Вот эта потом нужно будет поменять, наверное
			debug(RED"[command_oper] Имя введено неверно"DEFAULT);
			return ;
		}
		if (param[2] != PASSWORD_ADMIN) {
			debug(RED"[command_oper] Пароль неверный"DEFAULT);
			return ;
		}
		debug(GREEN"[command_oper] Вы стали IRC-оператором"DEFAULT);
		res->first->second.setModeO(true);
}
	// Возвращает список пользователей по маске за исключением невидимых
	void	command_who(pair<map<int, User>::iterator, bool> *res) {
		bool	flag = false;

		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_who] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam != 2 && lenparam != 3) {
			debug(RED"[command_who] Неверное число аргументов"DEFAULT);
			return ;
		}
		if (lenparam == 3 && param[2] != "o") {
			debug(RED"[command_who] Флаг установлен неправильно"DEFAULT);
			return ;
		}
		for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++) {
			if (check_mask(it1->second.getName(), param[1])) {
				debug(GREEN"[command_who] Найден пользователь подходящий под маску"DEFAULT);
				if (it1->second.getStatus() != 1)
					debug(RED"[command_who] Найденный пользователей не зарегестрирован"DEFAULT);
				else if (it1->second.getMode().i == true)
					debug(RED"[command_who] Найденный пользователей невидимый"DEFAULT);
				else if (lenparam == 3 && it1->second.getMode().o == false)
					debug(RED"[command_who] Найденный пользователей не IRC-оператор"DEFAULT);
				else
					add_message(id, ":"SERVER_NAME" "RPL_WHOREPLY" * "+ res->first->second.getName() + " " + res->first->second.getUserName() + " " +\
					res->first->second.getIp() + " "SERVER_NAME" " + it1->second.getName() + " H: 0 " + res->first->second.getRealName() + "\n");
					flag = true;
			}
		}
		if (!flag)
			add_message(id, ":"SERVER_NAME" "ERR_NOSUCHNICK" " + res->first->second.getName() + " " + param[1] + " :No such nick\n");
		add_message(id, ":"SERVER_NAME" "RPL_ENDOFWHO" " + res->first->second.getName() + " " + param[1] + " :End of /WHO list\n");
}
	// Используется для проверки наличия активности клиента на другом конце
	// Команду должен использовать сервер раз в какое-то время
	void	command_ping(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(2, res->first->second.getStatus()))
			return ;
		if (param[1] == SERVER_NAME)
			add_message(id, ":" + (string)SERVER_NAME" " + "PONG :"SERVER_NAME"\n");
		else
			debug(RED"[command_ping] Имя сервера неверно"DEFAULT);
	}
	// Используется для проверки наличия активности клиента на другом конце
	// Команда не посылает ответ пользователю, возможно она ещё что то должна делать
	void command_pong(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(2, res->first->second.getStatus()))
			return ;
		if (param[1] != SERVER_NAME)
			debug(RED"[command_pong] Имя сервера неверно"DEFAULT);
	}
	//=====================================================================================================================
	//=====================================================================================================================
	// Команда отправляет сообщения всем IRC-операторам, находящимся в сети
	void command_wallops(pair<map<int, User>::iterator, bool> *res) {
		if (!command_base_check(2, res->first->second.getStatus()))
				return ;
		if (res->first->second.getMode().o == false) {
			debug(RED"[command_wallops] Только оператор может пользоваться этой командой"DEFAULT);
			return ;
		}
		for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++) {
			if (it1->second.getStatus() != 1)
				debug(RED"[command_wallops] Пользователь " + it1->second.getName() + " не зарегестрирован"DEFAULT);
			else if (it1->second.getMode().o == false)
				debug(RED"[command_wallops] Пользователь " + it1->second.getName() + " не IRC-оператор"DEFAULT);
			else if (it1->second.getMode().w == true)
				debug(RED"[command_wallops] Пользователь " + it1->second.getName() + " отключил уведомления"DEFAULT);
			else
				add_message(it1->first, getFrontLine() + param[0] + " " + it1->second.getName() + " " + ((param[1][0] == ':') ? ("") : (":")) + param[1] + "\n");
		}
	}
	// Возвращает разные статусы каждого пользователя
	// std::to_string надо будет поменять
	void command_whois(pair<map<int, User>::iterator, bool> *res) {
		bool	flag = false;
		
		if (!command_base_check(2, res->first->second.getStatus()))
				return ;
		for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++) {
			if (check_mask(it1->second.getName(), param[1])) {
				debug(GREEN"[command_whois] Найден пользователь подходящий под маску"DEFAULT);
				if (it1->second.getStatus() != 1)
					debug(RED"[command_whois] Найденный пользователей не зарегестрирован"DEFAULT);
				else {
					add_message(id, ":"SERVER_NAME" "RPL_WHOISUSER" " + res->first->second.getName() + " " + it1->second.getName() + " " +
								res->first->second.getUserName() + " " + res->first->second.getIp() + " * :" + res->first->second.getRealName() + "\n");
					add_message(id, ":"SERVER_NAME" "RPL_WHOISCHANNELS" " + res->first->second.getName() + " " + it1->second.getName() + " :\n");
					add_message(id, ":"SERVER_NAME" "RPL_WHOISSERVER" " + res->first->second.getName() + " " + it1->second.getName() + " "
								SERVER_NAME" :IRC server based on TCP/IP protocol to rfc1459 standard\n");
					add_message(id, ":"SERVER_NAME" "RPL_WHOISIDLE" " + res->first->second.getName() + " " + it1->second.getName() + " " +
					std::to_string(res->first->second.getTimeIdle()) + " " + std::to_string(getCurrentTimeForUser()) + " :seconds idle\n");
					flag = true;
				}
			}
		}
		if (!flag)
			add_message(id, ":"SERVER_NAME" "ERR_NOSUCHNICK" " + res->first->second.getName() + " " + param[1] + " :No such nick\n");
		add_message(id, ":"SERVER_NAME" "RPL_ENDOFWHOIS" " + res->first->second.getName() + " " + param[1] + " :End of /WHOIS list\n");
	}
	// Возвращает информацию об имени пользователя, которое сейчас не используется
	// Вот эта команда очень зашкварная, либо пишите сами, либо давайте её выкинем, нифиг она нужна
	// void command_whowas(pair<map<int, User>::iterator, bool> *res) {}
	//=====================================================================================================================
	//==================================== КАНАЛЫ =========================================================================
	//=====================================================================================================================
	// Используется клиентом для входа на канал
	void command_join(pair<map<int, User>::iterator, bool> *res) {
		//Раскомментировать
		if (res->first->second.getStatus() != 1) {
			debug(RED"[command_join] Нельзя запросить информацию до полной регистрации"DEFAULT);
			return ;
		}
		if (lenparam != 2 && lenparam != 3) {
			debug(RED"[command_join] Неверное число аргументов"DEFAULT);
			return ;
		}
		parser_vector(param[1], &channel_list);
		if (lenparam == 3)
			parser_vector(param[2], &pass_list);
		//===============================================================	
		for (int i = 0; i < channel_list.size(); i++) {
			bool	flag = true;
			int		count = 0;
			for (vector<Channel>::iterator j = channel->begin(); j != channel->end(); j++, count++) {
				if (channel_list[i] == j->getName()) {
					debug(GREEN"[command_join] Канал " + channel_list[i] + " СУЩУСТВУЕТ, пытаюсь присоединиться..."DEFAULT);
					if (count < pass_list.size())
						j->addUser(id, pass_list[count]);
					else
						j->addUser(id);
					flag = false;
					break ;
				}
			}
			if (flag) {
				debug(GREEN"[command_join] Канал " + channel_list[i] + " НЕ СУЩУСТВУЕТ, пытаюсь создать..."DEFAULT);
				channel->push_back(Channel(channel_list[i], id));
			}
		}
	}
// Используется для изменения или просмотра топика канала
//	void command_topic(pair<map<int, User>::iterator, bool> *res) {}
// Используется для приглашения пользователей на канал
//	void command_invite(pair<map<int, User>::iterator, bool> *res) {}
// Исключает пользователя из канала может быть использована только оператором канала
//	void command_kick(pair<map<int, User>::iterator, bool> *res) {}
// Пользователь может покинуть каналы, которые он укажет в параметрах
//	void command_part(pair<map<int, User>::iterator, bool> *res) {}
// Пользователь может получить список всех пользователей, состоящих в канале
//	void command_names(pair<map<int, User>::iterator, bool> *res) {}
// Используется для вывода списка каналов и их топиков
//	void command_list(pair<map<int, User>::iterator, bool> *res) {}
//=====================================================================================================================
//=====================================================================================================================

public:
	string						str_message;
	int							id;
	map<int, User>				*clients;
	vector<string>				param;
	int 						lenparam;
	string						pass;
	map<int, string>			&clients_ivan;
	fd_set						&fds;
	set<string>					user_list;
	vector<string>				channel_list;
	vector<string>				pass_list;
	string						ip;
	bool						is_notice;
	vector<pair<int, string> >	messages;
	vector<Channel>				*channel;
	//====================================================================================================================================================
	// Конструктор
	MassegeHandler(int id, string str_message, map<int, User> *clients, string _pass, map<int, std::string> &clients_ivan, fd_set &fds, string ip, vector <Channel> *channel):
	id(id), str_message(str_message), clients(clients), pass(_pass), clients_ivan(clients_ivan), fds(fds), ip(ip), channel(channel), is_notice(false) {
		parser_param();
		lenparam = param.size();
		commands["PASS"] = &MassegeHandler::command_pass;
		commands["NICK"] = &MassegeHandler::command_nick;
		commands["USER"] = &MassegeHandler::command_user;
		commands["QUIT"] = &MassegeHandler::command_quit;
		commands["PRIVMSG"] = &MassegeHandler::command_privmsg;
		commands["AWAY"] = &MassegeHandler::command_away;
		commands["NOTICE"] = &MassegeHandler::command_notice;
		commands["MODE"] = &MassegeHandler::command_mode;
		commands["ISON"] = &MassegeHandler::command_ison;
		commands["USERHOST"] = &MassegeHandler::command_userhost;
		commands["VERSION"] = &MassegeHandler::command_version;
		commands["INFO"] = &MassegeHandler::command_info;
		commands["ADMIN"] = &MassegeHandler::command_admin;
		commands["TIME"] = &MassegeHandler::command_time;
		commands["OPER"] = &MassegeHandler::command_oper;
		commands["WHO"] = &MassegeHandler::command_who;
		commands["PING"] = &MassegeHandler::command_ping;
		commands["PONG"] = &MassegeHandler::command_pong;
		commands["WALLOPS"] = &MassegeHandler::command_wallops;
		commands["WHOIS"] = &MassegeHandler::command_whois;
		//commands["WHOWAS"] = &MassegeHandler::command_whowas; //скипаем
		//================================================
		//=======             КАНАЛЫ               =======
		//================================================
		//commands["TOPIC"] = &MassegeHandler::command_topic;
		commands["JOIN"] = &MassegeHandler::command_join;
		//commands["INVITE"] = &MassegeHandler::command_invite;
		//commands["KICK"] = &MassegeHandler::command_kick;
		//commands["PART"] = &MassegeHandler::command_part;
		//commands["NAMES"] = &MassegeHandler::command_names;
		//commands["LIST"] = &MassegeHandler::command_list;
	}
	// Распечатка
	void	printMassege(){
		string	temp = "";
		debug("[printMassege] Cообщение получено {" + str_message + "}");
		for (int i = 0; i < lenparam; i++)
			temp += "(" + param[i] + ") ";
		cout << "size user_list= " << user_list.size() << endl;
		for (set<string>::iterator us1 = user_list.begin(); us1 != user_list.end(); us1++)
			cout << "{" << *us1 << "}" << endl;
		cout << "size channel_list= " << channel_list.size() << endl;
		for (vector<string>::iterator us1 = channel_list.begin(); us1 != channel_list.end(); us1++)
			cout << "{" << *us1 << "}" << endl;
		cout << "size pass_list= " << pass_list.size() << endl;
		for (vector<string>::iterator us1 = pass_list.begin(); us1 != pass_list.end(); us1++)
			cout << "{" << *us1 << "}" << endl;
		if (DEBUG)
		{
			debug(clients->size() ? "[printMassege] Печать всех пользователей" : "[handle_message] Нет пользователей");
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++)
				it1->second.printUser();
			debug(clients->size() ? "[printMassege] Печать всех каналов" : "[handle_message] Нет пользователей");
			for (vector<Channel>::iterator it1 = channel->begin(); it1 != channel->end(); it1++)
				it1->printChannel();
			debug(lenparam ? "[printMassege] Аргументы: " + temp : RED"[handle_message] нет аргументов" DEFAULT);
		}
	}
	// Обработка
	vector <pair<int, string> > message() {
		User									user(id, ip);
		pair<map<int, User>::iterator, bool>	res = clients->insert(make_pair(id, user));

		res.first->second.setTimeIdle(getCurrentTimeForUser());
		if(lenparam > 0){
			try	{ (this->*commands.at(param[0]))(&res); }
			catch(const std::exception & e)	{ debug(RED"[handle_message] Неизвестная команда" DEFAULT); }
		}
		else
			debug(RED"[handle_message] в строке слишком мало параметров" DEFAULT);
		if (res.first->second.registration()){
			debug("[handle_message] Новый пользователь зарегистрирован");
			add_message(id, getFrontLineRPL("- " + (string)SERVER_NAME + " Message of the day -\n", RPL_MOTDSTART));
			add_message(id, getFrontLineRPL("Регистрация пройдена\n", RPL_MOTD));
			add_message(id, getFrontLineRPL("End of /MOTD command\n", RPL_ENDOFMOTD));
		}
		// for (int i = 0; i < 10; i++) {
		// 	cout << YELLOW"Обработка..."DEFAULT << endl;
		// 	usleep(1000000);
		// }
		return (messages);
	}
};
