#include "../../general.hpp"
#include "../../User.hpp"

//int	handle_user(const string str_message, string pass, User *user);

string	handle_start(string str_message, int id, map<int, User> *clients_map, string pass)
{
	pair<map<int, User>::iterator, bool> res;
	User	user(id);

	debug("Cообщение получено {" + str_message + "}");

	res = clients_map->insert(make_pair(id, user));
	res.second ? cout << "Добавлен новый пользователь\n" : cout << "Нет новых пользователей\n";

	if (str_message.substr(0, 5) == "PASS ")
		res.first->second.setPass(str_message.substr(5, str_message.size()), pass);
	else if (str_message.substr(0, 5) == "NICK ")
	{
		map<int, User>::iterator	it1 = clients_map->begin(), it2 = clients_map->end();
		string						nick = str_message.substr(5, str_message.size());

		for (; it1 != it2; it1++)
		{
			if (it1->second.getName() == nick)
			{
				cout << "Такой ник уже есть\n";
				break ;
			}
		}
		if (it1 == it2)
			res.first->second.setNick(nick);
	}
	else if (str_message.substr(0, 5) == "USER ")
		res.first->second.setData(str_message.substr(5, str_message.size()));


	//res = rand() % 2;
	return (res.second ? GREEN "OK\n" DEFAULT : RED "KO\n" DEFAULT);
}

int main()
{
	cout << "Hello Test\n";

	map<int, User> clients_map;
	string	pass = "123";

	// cout << handle_start("PASS 123", 4, &clients_map, pass);
	// cout << handle_start("PASS 123", 5, &clients_map, pass);

	cout << handle_start("NICK   ALEX", 4, &clients_map, pass);
	cout << handle_start("PASS   123", 4, &clients_map, pass);

	cout << handle_start("USER lol  kek 123    Hello", 4, &clients_map, pass);
	for (map<int, User>::iterator it1 = clients_map.begin(); it1 != clients_map.end(); it1++)
		it1->second.printUser();

	

	cout << "========================\n";
	// for (map<int, User>::iterator it1 = clients_map.begin(); it1 != clients_map.end(); it1++)
	// 	it1->second.printUser();
	// clients_map->printUserMap();

	return (0);
}
