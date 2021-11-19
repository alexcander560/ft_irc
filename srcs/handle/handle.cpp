#include "../../general.hpp"

// Разбивают строку на набор параметров
void	parser_param(string str_message, vector<string> *param)
{
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
				param->push_back(temp);
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
		param->push_back(temp);
}

pair<int, string>	handle_message(string str_message, int id, map<int, User> *clients_map, string pass)
{
	pair<map<int, User>::iterator, bool>	res;			// для хранения возвращаемого значения insert
	User									user(id);		// пользователь, который будет добавлен в набор
	string									temp;			// промежуточные данные
	vector<string>							param;			// исходная строка, поделённая на параметры
	int										len_param;		// число параметров
	pair<int, string>						output;			// вывод

	output = make_pair(-1, "Сообщения нет, какая жалость\n");

	//debug("[handle_message] =======================================================");
	parser_param(str_message, &param);
	len_param = param.size();
	// Печать строки
	debug("[handle_message] Cообщение получено {" + str_message + "}");
	for (int i = 0; i < len_param; i++)
		temp += "(" + param[i] + ") ";
	debug(len_param ? "[handle_message] Аргументы: " + temp : RED"[handle_message] нет аргументов"DEFAULT);
	// Создание пользователей
	res = clients_map->insert(make_pair(id, user));
	debug(res.second ? "[handle_message] Добавлен новый пользователь" : "[handle_message] Нет новых пользователей");
	// Обработка параметров
	if (param.size() > 0)
	{
		if (param[0] == "PASS")
		{
			if (res.first->second.getStatus() == -1)
				res.first->second.setPass(param, pass);
			else
				cout << "Вы уже зарегистрированы!\n";
		}
		else if (param[0] == "NICK")
		{
			if (len_param != 2)
				debug(RED"[handle_message] Неверное число аргументов для команды NICK"DEFAULT);
			else
			{
				map<int, User>::iterator	it1 = clients_map->begin(), it2 = clients_map->end();

				for (; it1 != it2; it1++)
				{
					if (it1->second.getName() == param[1])
					{
						debug(RED"[handle_message] Такой NICK уже есть"DEFAULT);
						break ;
					}
				}
				if (it1 == it2)
					res.first->second.setNick(param);
			}
		}
		else if (param[0] == "USER")
			res.first->second.setData(param);
		else if (param[0] == "OPER")
			debug(RED"[handle_message] Заглушка"DEFAULT);
		else if (param[0] == "QUIT")
		{
			clients_map->erase(id);
			debug("[handle_message] Пользователь удалён");
		}
		else if (param[0] == "PRIVMSG")
		{
			if (len_param != 3)
				debug(RED"[handle_message] PRIVMSG неверное число аргументов"DEFAULT);
			else
			{
				if (res.first->second.getStatus() == 1)
				{
					map<int, User>::iterator	it1 = clients_map->begin(), it2 = clients_map->end();

					for (; it1 != it2; it1++)
					{
						if (it1->second.getName() == param[1])
						{
							if (it1->second.getStatus() == -1)
								debug(RED"[handle_message] Пользователь с таким ником не прошёл полную регистрацию"DEFAULT);
							else
							{
								debug("[handle_message] Пользователь найден, отправляю сообщение...");
								output = make_pair(it1->first, param[2] + "\n");
							}
							break ;
						}
					}
					if (it1 == it2)
						debug(RED"[handle_message] Пользователь с таким ником не найден"DEFAULT);
				}
				else
					debug(RED"[handle_message] Нельзя отправить сообщение до регистрации"DEFAULT);
			}
		}
		else
			debug(RED"[handle_message] Неизвестная команда"DEFAULT);

		// Регистрация пользователей
		if (res.first->second.registration())
			debug("[handle_message] Новый пользователь зарегистрирован");
	}
	else
		debug(RED"[handle_message] в строке слишком мало параметров"DEFAULT);

	if (DEBUG)
	{
		debug(clients_map->size() ? "[handle_message] Печать всех пользователей" : "[handle_message] Нет пользователей");
		for (map<int, User>::iterator it1 = clients_map->begin(); it1 != clients_map->end(); it1++)
			it1->second.printUser();
	}
	debug("[handle_message] =============================================================================");
	return (output);
}

/*
int main()
{
	//cout << "Hello Test\n";

	map<int, User> clients_map;
	string	pass = "123";
	pair<int, string> output;

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ PASS----------------
	//-----------------------------------------------
	// handle_message("PASS 1234 7", 5, &clients_map, pass);
	// handle_message("PASS 1234", 5, &clients_map, pass);
	handle_message("PASS 123  ", 4, &clients_map, pass);					//Вариант для полной работы
	handle_message("PASS 123", 5, &clients_map, pass);					//Вариант для полной работы

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ NICK----------------
	//-----------------------------------------------
	// handle_message("NICk Alex", 5, &clients_map, pass);
	// handle_message("NICK Alex COOL", 5, &clients_map, pass);
	// handle_message("NICK Alex-COOLL", 5, &clients_map, pass);
	// handle_message("NICK Alex!", 5, &clients_map, pass);
	handle_message("NICK Oleg", 4, &clients_map, pass);
	handle_message("NICK Alex", 5, &clients_map, pass);					//Вариант для полной работы
	//handle_message("NICK Alex", 4, &clients_map, pass);

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ USER----------------
	//-----------------------------------------------
	// handle_message("USER 10 20 30", 5, &clients_map, pass);
	// handle_message("USER 10 20 30 40 50", 5, &clients_map, pass);
	// handle_message("USER 10 20 30 40", 5, &clients_map, pass);
	handle_message("USER * * * *", 4, &clients_map, pass);			//Вариант для полной работы
	handle_message("USER * * * *", 5, &clients_map, pass);			//Вариант для полной работы

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ OPER----------------
	//-----------------------------------------------
	//handle_message("OPER ???", 4, &clients_map, pass);

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ QUIT----------------
	//-----------------------------------------------
	//handle_message("QUIT", 4, &clients_map, pass);
	// handle_message("QUIT buy", 4, &clients_map, pass);
	// handle_message("QUIT buy buy", 4, &clients_map, pass);

	//-----------------------------------------------
	//--------------БЛОК ТЕСТОВ PRIVMSG--------------
	//-----------------------------------------------
	// handle_message("PRIVMSG", 4, &clients_map, pass);
	// handle_message("PRIVMSG Hello", 4, &clients_map, pass);
	output = handle_message("PRIVMSG Oleg Hello", 5, &clients_map, pass);

	cout << output.first << ": " << output.second << endl;


	return (0);
}
*/