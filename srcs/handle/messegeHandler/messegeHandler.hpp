#pragma once
#include "../../general.hpp"
#include "../../listen/support.hpp"
#include "additions.hpp"
#include "../general/answers.hpp"
//#include "../users/user.hpp"

#define SERVER_NAME "DragonsCHAT"
#define SERVER_VERSION "release (1.12.2)"

class MassegeHandler {
private:
	const string	getFrontLine(int user_id = -1) const //Использовать с аргументом по умолчанию, когда хотим получить строку с данными ЭТОГО юзера. Подавать user_id в иных случаях
	{
		if (user_id == -1)
			user_id = id;
		string	user = this->clients->find(user_id)->second.getName();
		string	name = this->clients->find(user_id)->second.getUserName();
		string	ipaddress = this->clients->find(user_id)->second.getIp();
		return (string(":") + user + string("!") + name + string("@") + ipaddress + string(" "));
	}

	const string	getFrontLineRPL(string value, string rpl, int user_id = -1)
	{
		if (user_id == -1)
			user_id = id;
		string user = this->clients->find(user_id)->second.getName();
		return (":"SERVER_NAME" " + rpl + " " + user + " "SERVER_NAME" :" + value);
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
		string	current_name("");
		int		i = 0;
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
	vector< pair<int, string> > command_privmsg(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) {
		if (lenparam != 3)
			debug(RED"[command_privmsg] PRIVMSG неверное число аргументов" DEFAULT);
		else {
			if (res->first->second.getStatus() == 1)
			{
				if (param.at(1) == "bot") {	/* IVAN - FOR BOT */
					debug("[command_privmsg] Calling bot with command");
					handle_command(param.at(2), id, clients->find(id)->second.getName(), message);
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
									debug(RED"[command_privmsg] Пользователь с таким ником не прошёл полную регистрацию" DEFAULT);
								else if (it1->second.getMode().s)
									debug("[command_privmsg] У конечной точки стоит флаг +s. NOTICE не сработает!");
								else
								{
									debug("[command_privmsg] Пользователь найден, отправляю сообщение...");
									message->push_back(make_pair(it1->first, getFrontLine() + param[0] + " " + *us1 + " " + ((param[2][0] == ':') ? ("") : (":")) + param[2] + "\n") );
									/* Check AWAY - BEGIN */
									if (!is_notice && it1->second.getAwayMessage().first)
									{
										debug("Away automessage was add");
										message->push_back(make_pair(id, getFrontLine(it1->first) + "PRIVMSG " + clients->find(id)->second.getName() + " :" + it1->second.getAwayMessage().second + "\n"));
									}
									else if (is_notice)
										debug("It's not need to send automessage, because it's notice");
									else
										debug("It's not need to send automessage");
									/* Check AWAY - END */
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
	vector< pair<int, string> > command_away(pair<map<int, User>::iterator, bool> *res, vector< pair<int, string> > *message) //ГОТОВА 100%
	{
		if (lenparam == 1)
		{
			debug("[command_away] Away message was unset");
			clients->find(id)->second.setAwayMessage(make_pair(false, ""));
		}
		else if (lenparam == 2)
		{
			debug("[command_away] Away message was set");
			clients->find(id)->second.setAwayMessage(make_pair(true, param[1]));
		}
		else
			warning(RED"[command_away] Неверное число аргументов" DEFAULT);
		return (*message);
	}
	vector< pair<int, string> > command_notice(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message)
	{
		if (lenparam == 3)
		{
			is_notice = true;
			command_privmsg(res, message);
			is_notice = false;
		}
		else
			debug(RED"[command_notice] Неверное число аргументов"DEFAULT);
		return (*message);
	}
	vector< pair<int, string> > command_mode(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {
		if (lenparam == 2)
		{
			if (res->first->second.getStatus() == 1)
			{
				if (param[1].size() != 2)
					debug(RED"[command_mode] Строка с параметрами неверной длины"DEFAULT);
				else if (param[1][0] != '+' && param[1][0] != '-')
					debug(RED"[command_mode] Знак mode неверный {" + string(1, param[1][0]) + "}"DEFAULT);
				else
				{
					if (param[1][1] == 'i')
						res->first->second.setModeI(param[1][0] == '+' ? true : false);
					else if (param[1][1] == 'w')
						res->first->second.setModeW(param[1][0] == '+' ? true : false);
					else if (param[1][1] == 's')
						res->first->second.setModeS(param[1][0] == '+' ? true : false);
					else if (param[1][1] == 'o')
						res->first->second.setModeO(param[1][0] == '+' ? true : false);
					else
						debug(RED"[command_mode] флаг mode неверный {" + string(1, param[1][1]) + "}"DEFAULT);
				}
			}
			else
				debug(RED"[command_mode] Нельзя менять статусы до полной регистрации"DEFAULT);
		}
		else
			debug(RED"[command_mode] Неверное число аргументов"DEFAULT);
		return *message;
	}
	vector< pair<int, string> > command_info(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {
		if (lenparam == 1)
		{
			if (res->first->second.getStatus() == 1)
			{
				message->push_back(make_pair(id, getFrontLineRPL("describe\n", RPL_INFO)));
				message->push_back(make_pair(id, getFrontLineRPL("IRC server based on TCP/IP protocol to rfc1459 standard\n", RPL_INFO)));
				message->push_back(make_pair(id, getFrontLineRPL("End of /INFO list\n", RPL_ENDOFINFO)));
			}
			else
				debug(RED"[command_info] Нельзя запросить информацию до полной регистрации"DEFAULT);
		}
		else
			debug(RED"[command_info] Неверное число аргументов"DEFAULT);
		return *message;
	}

//	vector< pair<int, string> > command_who(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_whois(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_whowas(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_topic(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_join(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_invite(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_kick(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_part(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_names(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_list(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_wallops(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_ping(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_pong(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_ison(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_userhost(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}

//	vector< pair<int, string> > command_version(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
	vector< pair<int, string> > command_version(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message)
	{
		if (res->first->second.getStatus() != 1)
		{
			debug(RED"[command_version] Нельзя запросить информацию до полной регистрации"DEFAULT);
			goto done;
		}
		if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME))
			message->push_back(make_pair(id, getFrontLineRPL(SERVER_VERSION, RPL_VERSION) + "\n"));
		else
			debug(RED"[command_version] No such server"DEFAULT);
	done:
		return (*message);
	}
//	vector< pair<int, string> > command_info(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
//	vector< pair<int, string> > command_admin(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message) {}
	vector< pair<int, string> > command_time(pair<map<int, User>::iterator, bool> *res,vector< pair<int, string> > *message)
	{
		if (res->first->second.getStatus() != 1)
		{
			debug(RED"[command_info] Нельзя запросить информацию до полной регистрации"DEFAULT);
			goto done;
		}
		if (lenparam == 1 || (lenparam == 2 && param[1] == SERVER_NAME))
			message->push_back(make_pair(id, getFrontLineRPL(getCurrentTime(), RPL_TIME)));
		else
			debug(RED"[command_time] No such server"DEFAULT);
	done:
		return (*message);
	}
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
	bool				is_notice;

	// Конструктор
	MassegeHandler(int id, string str_message, map<int, User> *clients, string _pass,
	map<int, std::string> &clients_ivan, fd_set &fds, string ip):
	id(id), str_message(str_message), clients(clients), pass(_pass),
	clients_ivan(clients_ivan), fds(fds), ip(ip), is_notice(false) {
		_parser_param();
		lenparam = param.size();
		commands["PASS"] = &MassegeHandler::command_pass;
		commands["NICK"] = &MassegeHandler::command_nick;
		commands["USER"] = &MassegeHandler::command_user;
		commands["QUIT"] = &MassegeHandler::command_quit;
		commands["PRIVMSG"] = &MassegeHandler::command_privmsg;
		commands["AWAY"] = &MassegeHandler::command_away;
		commands["NOTICE"] = &MassegeHandler::command_notice;
		commands["MODE"] = &MassegeHandler::command_mode;
		commands["VERSION"] = &MassegeHandler::command_version;
		commands["TIME"] = &MassegeHandler::command_time;
		commands["INFO"] = &MassegeHandler::command_info;
	}
	// Распечатка
	void printMassege() {
		string	temp = "";
		debug("[printMassege] Cообщение получено {" + str_message + "}");
		for (int i = 0; i < lenparam; i++)
			temp += "(" + param[i] + ") ";
		if (DEBUG)
		{
			debug(clients->size() ? "[printMassege] Печать всех пользователей" : "[handle_message] Нет пользователей");
			for (map<int, User>::iterator it1 = clients->begin(); it1 != clients->end(); it1++)
				it1->second.printUser();
			debug(lenparam ? "[printMassege] Аргументы: " + temp : RED"[handle_message] нет аргументов" DEFAULT);
		}
		debug("[printMassege] =============================================================================");
	}
	// Обработка
	vector< pair<int, string> > message() {
		vector< pair<int, string> >				messages;
		User									user(id, ip);
		pair<map<int, User>::iterator, bool>	res = clients->insert(make_pair(id, user));

		if(lenparam > 0){
			try {
				messages = (this->*commands.at(param[0])) (&res, &messages);
			} catch(const std::exception & e) {
				debug(RED"[handle_message] Неизвестная команда" DEFAULT);	
			}
		}
		else
			debug(RED"[handle_message] в строке слишком мало параметров" DEFAULT);
		if (res.first->second.registration())
		{
			debug("[handle_message] Новый пользователь зарегистрирован");
			messages.push_back(make_pair(id, getFrontLineRPL("- DragonsCHAT Message of the day -\n", RPL_MOTDSTART)));
			messages.push_back(make_pair(id, getFrontLineRPL("Регистрация пройдена\n", RPL_MOTD)));
			messages.push_back(make_pair(id, getFrontLineRPL("End of /MOTD command\n", RPL_ENDOFMOTD)));
		}
		return (messages);
	} 
};
