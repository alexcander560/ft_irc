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
	bool	parser_user(string str) {
		string	current_name("");
		int		i = 0;

		while (str[i]) {
			if (str[i] == ',') {
				if (current_name.size() > 0) {
					if (!(user_list.insert(current_name)).second)
						debug(RED"[parser_user] Двойнок ник");
					current_name.clear();
				}
			}
			else
				current_name.push_back(str[i]);
			i++;
		}
		if (current_name.size() > 0)
			if (!(user_list.insert(current_name)).second)
				debug(RED"[parser_user] Двойнок ник " + current_name + DEFAULT);
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
			debug(RED"[command_base_check] " + param[0] + "Нельзя выполнить до регистрации пользователя" DEFAULT);
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
		parser_user(param[1]);
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
					res->first->second.setModeO(param[2][0] == '+' ? true : false);
				else
					debug(RED"[command_mode] флаг mode неверный {" + string(1, param[2][1]) + "}"DEFAULT);
			}
		}
		else
			debug(RED"[command_mode] Имя пользователя введено неверно"DEFAULT);
	}
	// Возвращает список пользователей в сети
	// У нас же он просто возвращет набор пользователей, которые есть
	// Даже если пользователей не в сети, да даже если он не зарегистрирвоан, просто насрать
	// Наверное ещё дубликаты надо выкидывать, но это неоднозначно
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
					// else if (/*Что такое пользователь в сети, я уже хз*/)
					// 	debug(RED"[command_ison] Пользователь с ником " + param[current] + " не в сети"DEFAULT);
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
	void command_userhost(pair<map<int, User>::iterator, bool> *res) {
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
	void command_version(pair<map<int, User>::iterator, bool> *res) {
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
	void command_info(pair<map<int, User>::iterator, bool> *res) {
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
	void command_admin(pair<map<int, User>::iterator, bool> *res) {
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
	void command_time(pair<map<int, User>::iterator, bool> *res) {
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

//=====================================================================================================================
//=====================================================================================================================
// Взятие операторских прав
// void command_oper(pair<map<int, User>::iterator, bool> *res) {}
// Возвращает список пользователей за исключением невидимых
//	void command_who(pair<map<int, User>::iterator, bool> *res) {}
// Возвращает разные статусы каждого опользователя
//	void command_whois(pair<map<int, User>::iterator, bool> *res) {}
// Возвращает информацию об имени пользователя, которое сейчас не используется
//	void command_whowas(pair<map<int, User>::iterator, bool> *res) {}
// Используется для изменения или просмотра топика канала
//	void command_topic(pair<map<int, User>::iterator, bool> *res) {}
// Используется клиентом для входа на канал
//	void command_join(pair<map<int, User>::iterator, bool> *res) {}
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
// Команда отправляет сообщения всем IRC-операторам, находящимся в сети
//	void command_wallops(pair<map<int, User>::iterator, bool> *res) {}
// Используется для проверки наличия активности клиента на другом конце
//	void command_ping(pair<map<int, User>::iterator, bool> *res) {}
// Используется для проверки наличия активности клиента на другом конце
//	void command_pong(pair<map<int, User>::iterator, bool> *res) {}
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
	string						ip;
	bool						is_notice;
	vector< pair<int, string> >	messages;
	//====================================================================================================================================================
	// Конструктор
	MassegeHandler(int id, string str_message, map<int, User> *clients, string _pass, map<int, std::string> &clients_ivan, fd_set &fds, string ip):
	id(id), str_message(str_message), clients(clients), pass(_pass), clients_ivan(clients_ivan), fds(fds), ip(ip), is_notice(false) {
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
		if (DEBUG)
		{
			debug(clients->size() ? "[printMassege] Печать всех пользователей" : "[handle_message] Нет пользователей");
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++)
				it1->second.printUser();
			debug(lenparam ? "[printMassege] Аргументы: " + temp : RED"[handle_message] нет аргументов" DEFAULT);
		}
	}
	// Обработка
	vector <pair<int, string> > message(){
		User									user(id, ip);
		pair<map<int, User>::iterator, bool>	res = clients->insert(make_pair(id, user));

		res.first->second.setTime(getCurrentTimeForUser());
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
		return (messages);
	}
};
