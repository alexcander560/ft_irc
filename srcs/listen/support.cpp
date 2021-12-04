#include "support.hpp"

/* Поддерживающие функции для более удобного расположения кода */
/* Две функции - обработка ошибок, и еще две - для работы с буффером по ID пользователя */
/* UPD: Добавлена функция отправки сообщения по ID пользователя */
/* P.S. Драконы */

/* Отправить сообщение для пользователя по его ID */
void	send_message(std::vector< pair<int, string> > message)
{
	size_t	send_value;
	pair<int, string>	value;
	std::vector< pair<int, string> >::iterator	begin = message.begin();
	std::vector< pair<int, string> >::iterator	end = message.end();

	while (begin != end)
	{
		value = *begin;
		send_value = send(value.first, value.second.c_str(), value.second.size(), 0);
		if (send_value != -1 && DEBUG) {
			debug("[send_message] Message was send successful");
		}
		if (send_value == -1)
			warning("[send_message] Message cannot send. Unknown and not fatal error");
		begin++;
	}
}

/* Отключить пользователя от прослушивания сервером по его ID */
void	disconnect_by_id(int id, std::map<int, std::string> &clients, fd_set &fds)
{
	std::cout	<< "DEBUG: [listen_clients] Client with ID "
				 << id << " was disconnected. Active users count: "
				 << clients.size() - 1 << std::endl;
	close(id);
	FD_CLR(id, &fds);
	clients.erase(id);
}

/* Очистить строку ввода по ID пользователя */
void 	clear_by_id(int id, std::map<int, std::string> &clients)
{
	std::map<int, std::string>::iterator element = clients.find(id);
	if (element != clients.end())
		(*element).second.clear();
}

/* Добавить символ в общий ввод пользователя для формирования полноценной строки, потому как читаем посимвольно */
std::string	&add_character_by_id(int id, int character, std::map<int, std::string> &clients)
{
	std::map<int, std::string>::iterator element = clients.find(id);
	(*element).second.push_back(character);
	return ((*element).second);
}

/* Обработка ошибок для функции select() */
void	handle_select(const int select_value)
{
	if (select_value != -1)
		return ;
	fatal(std::strerror(errno));
}

/* Обработка ошибок для функции accept() */
void	handle_accept(const int select_accept)
{
	if (select_accept != -1)
		return ;
	fatal(std::strerror(errno));
}
