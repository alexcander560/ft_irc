#pragma once
#include "../../general.hpp"
#include "../../listen/support.hpp"
//#include "../users/user.hpp"


class MassegeHandler {
private:
	const string	getFrontLine() const
	{
		string user = this->clients->find(this->id)->second.getName();
		string name = this->clients->find(this->id)->second.getUserName();
		string ipaddress = this->clients->find(this->id)->second.getIp();
		return (string(":") + user + string("!") + name + string("@") + ipaddress + string(" "));
	}

	typedef vector< pair<int, string> > (MassegeHandler::*Method) (pair<map<int, User>::iterator, bool> *, vector< pair<int, string> > *);
	map<string, Method> commands;
	void _parser_param() {
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
		for (int i = 0; i < len; i++)
		{
			if (str_message[i] == ' ')
			{
				if (flag == true)
				{
					param.push_back(temp);
					temp = "";
					flag = false;
				}
				while (str_message[i] && str_message[i] == ' ')
					i++;
				i--;
			}
			else
			{
				if (str_message[i] == ':' && flag == false)
				{
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
	bool _parser_user(string str){
		

		/*
		ПРОВЕРКОЙ НА ДВОЙНОЙ НИК
		*/

		//user_list
		std::string	current_name("");
		int	i = 0;


		while (str[i])
		{
			if (str[i] == ',')
			{
				if (current_name.size() > 0)
				{
					if (!(user_list.insert(current_name)).second)
						debug(RED"[_parser_user] Двойнок ник");
					current_name.clear();
				}
			}
			else
				current_name.push_back(str[i]);
			i++;
		}
		if (current_name.size() > 0)
			if (!(user_list.insert(current_name)).second)
				debug(RED"[_parser_user] Двойнок ник " + current_name + DEFAULT);
		return (true);
	}
	vector< pair<int, string> > command_pass(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {
		if (res->first->second.getStatus() == -1)
			res->first->second.setPass(param, pass);
		else
			debug(RED"[handle_message] Вы уже зарегистрированы!" DEFAULT);

		return *message;
	}
	vector< pair<int, string> > command_nick(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {

		if (lenparam != 2)
			debug(RED"[handle_message] Неверное число аргументов для команды NICK" DEFAULT);
		else {
			map<int, User>::iterator	it1 = clients->begin(), it2 = clients->end();

			for (; it1 != it2; it1++){
				if (it1->second.getName() == param[1]){
					debug(RED"[handle_message] Такой NICK уже есть" DEFAULT);
					break ;
				}
			}
			if (it1 == it2)
				res->first->second.setNick(param);
		}

		return *message;
	}
	vector< pair<int, string> > command_user(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {
		res->first->second.setData(param);

		return *message;
	}
	vector< pair<int, string> > command_quit(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {
		clients->erase(id);
		disconnect_by_id(this->id, this->clients_ivan, this->fds);
		debug("Command QUIT was use for user");
		return *message;
	}

	// pair<int, string> command_oper(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	vector< pair<int, string> > command_privmsg(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {
		if (lenparam != 3)
			debug(RED"[handle_message] PRIVMSG неверное число аргументов" DEFAULT);
		else {
			if (res->first->second.getStatus() == 1)
			{
				if (param.at(1) == "bot") {	/* IVAN - FOR BOT */
					debug("[handle_message] Calling bot with command");
					message->push_back(make_pair(id, getFrontLine() + handle_command(param.at(2))));
				}
				else {
					_parser_user(param[1]);
					set<string>::iterator		us1 = user_list.begin(), us2 = user_list.end();
					//----------------------------
					cout << "size user_list= " << user_list.size() << endl;
					for (set<string>::iterator us1 = user_list.begin(); us1 != user_list.end(); us1++)
						cout << "{" << *us1 << "}" << endl;
					//----------------------------
					for (; us1 != us2; us1++){
						map<int, User>::iterator	it1 = clients->begin(), it2 = clients->end();
						for (; it1 != it2; it1++) {
							if (it1->second.getName() == *us1) {
								if (it1->second.getStatus() == -1)
									debug(RED"[handle_message] Пользователь с таким ником не прошёл полную регистрацию" DEFAULT);
								else
								{
									debug("[handle_message] Пользователь найден, отправляю сообщение...");
									//std::cout << "ааааааа Ж " << it1->first << endl;
									message->push_back(make_pair(it1->first, getFrontLine() + param[0] + " " + *us1 + " " + ((param[2][0] == ':') ? ("") : (":")) + param[2] + "\n") );
								}
								break ;
							}
						}
						if (it1 == it2)
							debug(RED"[handle_message] Пользователь с таким ником не найден" DEFAULT);
					}
				}
			}
			else
				debug(RED"[handle_message] Нельзя отправить сообщение до регистрации" DEFAULT);
		}

		return *message;
	}
	// pair<int, string> command_away(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_notice(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_who(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_whois(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_whowas(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_mode(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_topic(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_join(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_invite(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_kick(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_part(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_names(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_list(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_wallops(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_ping(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_pong(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_ison(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_userhost(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_version(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_info(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_admin(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
	// pair<int, string> command_time(pair<map<int, User>::iterator, bool> *res, pair<int, string> *message) {}
public:
	string				str_message;
	int					id;
	map<int, User>		*clients;
	vector<string>		param;
	int 				lenparam;
	string				pass;
	map<int, string>	&clients_ivan;
	fd_set				&fds;
	set<string>			user_list;
	string				ip;

	// Конструктор
	MassegeHandler(int id, string str_message, map<int, User> *clients, string _pass,
	map<int, std::string> &clients_ivan, fd_set &fds, string ip):
	id(id), str_message(str_message), clients(clients), pass(_pass),
	clients_ivan(clients_ivan), fds(fds), ip(ip) {
		_parser_param();
		lenparam = param.size();
		commands["PASS"] = &MassegeHandler::command_pass;
		commands["NICK"] = &MassegeHandler::command_nick;
		commands["USER"] = &MassegeHandler::command_user;
		commands["QUIT"] = &MassegeHandler::command_quit;
		commands["PRIVMSG"] = &MassegeHandler::command_privmsg;
	}
	// Распечатка
	void printMassege() {
		string temp = "";
		debug("[handle_message] Cообщение получено {" + str_message + "}");
		for (int i = 0; i < lenparam; i++)
			temp += "(" + param[i] + ") ";
		if (DEBUG)
		{
			debug(clients->size() ? "[handle_message] Печать всех пользователей" : "[handle_message] Нет пользователей");
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++)
				it1->second.printUser();
			debug(lenparam ? "[handle_message] Аргументы: " + temp : RED"[handle_message] нет аргументов" DEFAULT);
		}
		debug("[handle_message] =============================================================================");
	}
	// Обработка
	vector< pair<int, string> > message() {
		vector< pair<int, string> >		messages;
		User									user(id, ip);
		pair<map<int, User>::iterator, bool>	res = clients->insert(make_pair(id, user));

		if(lenparam > 0){
			try {
				messages = (this->*commands.at(param[0])) (&res, &messages);
			} catch(const std::exception & e) {
				debug(RED"[handle_message] Неизвестная команда" DEFAULT);	
			}
			// if (param[0] == "PASS")
			// 	message = command_pass(&res, &message);
			// else if (param[0] == "NICK")
			// 	message = command_nick(&res, &message);
			// else if (param[0] == "USER")
			// 	message = command_user(&res, &message);
			// else if (param[0] == "QUIT")
			// 	message = command_quit(&res, &message);
			// else if (param[0] == "PRIVMSG")
			// 	message = command_privmsg(&res, &message);
			// else
			// 	debug(RED"[handle_message] Неизвестная команда" DEFAULT);
		}
		else
			debug(RED"[handle_message] в строке слишком мало параметров" DEFAULT);

		if (res.first->second.registration())
			debug("[handle_message] Новый пользователь зарегистрирован");
		return (messages);
	} 
};
