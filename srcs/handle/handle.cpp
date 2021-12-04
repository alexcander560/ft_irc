#include "../general.hpp"
#include "messegeHandler/messegeHandler.hpp"

/* Обработка сообщения в str_message */
vector< pair<int, string> >	handle_message(string str_message, int id, map<int, User> *clients_map, string pass, map<int, string> &clients, fd_set &fds, string ip)
{
	vector< pair<int, string> >	output;

	MassegeHandler Handler(id, str_message, clients_map, pass, clients, fds, ip);
	output = Handler.message();

	// Распечатка
	Handler.printMassege();
	return (output);
}
