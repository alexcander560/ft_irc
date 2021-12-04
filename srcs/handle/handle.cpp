#include "../general.hpp"
#include "messegeHandler/messegeHandler.hpp"

/* Обработка сообщения в str_message */
vector< pair<int, string> >	handle_message(string str_message, int id, map<int, User> *clients_map, string pass, map<int, string> &clients, fd_set &fds)
{
	vector< pair<int, string> >	output;

	MassegeHandler Handler(id, str_message, clients_map, pass, clients, fds);
	output = Handler.message();

	// Распечатка
	Handler.printMassege();
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
	// handle_message("NICK Oleg", 4, &clients_map, pass);
	// handle_message("NICK Alex", 5, &clients_map, pass);					//Вариант для полной работы
	//handle_message("NICK Alex", 4, &clients_map, pass);

	//-----------------------------------------------
	//---------------БЛОК ТЕСТОВ USER----------------
	//-----------------------------------------------
	// handle_message("USER 10 20 30", 5, &clients_map, pass);
	// handle_message("USER 10 20 30 40 50", 5, &clients_map, pass);
	// handle_message("USER 10 20 30 40", 5, &clients_map, pass);
	// handle_message("USER * * * *", 4, &clients_map, pass);			//Вариант для полной работы
	// handle_message("USER * * * *", 5, &clients_map, pass);			//Вариант для полной работы

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
	//output = handle_message("PRIVMSG Oleg Hello", 5, &clients_map, pass);

	//cout << output.first << ": " << output.second << endl;


	return (0);
}
*/
