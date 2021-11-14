#include "support.hpp"

/* Поддерживающие функции для более удобного расположения кода */
/* Две функции - обработка ошибок, и еще две - для работы с буффером по ID пользователя */
/* UPD: Добавлена функция отправки сообщения по ID пользователя */
/* P.S. Драконы */

/* Отправить сообщение для пользователя по его ID */
void	send_message(const int id, std::string message)
{
	size_t	send_value;

	send_value = send(id, message.c_str(), message.size(), 0);
	if (send_value != -1 && DEBUG)
		debug("[send_message] Message was send successful");
	if (send_value == -1)
		warning("[send_message] Message cannot send. Unknown and not fatal error");
}

/* Очистить строку ввода по ID пользователя */
void 	clear_by_id(int id, std::map<int, std::string> &clients)
{
	std::map<int, std::string>::iterator element = clients.find(id);
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
	switch (errno)
	{
		case (EBADF):
			fatal("An invalid file descriptor was given in one of the sets. (Perhaps a file descriptor that was already closed, or one on which an error has occurred.)");
			break ;
		case (EINTR):
			fatal("A signal was caught");
			break ;
		case (EINVAL):
			fatal("The value contained within timeout is invalid.");
			break ;
		case (ENOMEM):
			fatal("Unable to allocate memory for internal tables.");
			break ;
	}
}

/* Обработка ошибок для функции accept() */
void	handle_accept(const int select_accept)
{
	if (select_accept != -1)
		return ;
	switch (errno)
	{
		case (EAGAIN):
			fatal("The socket is marked nonblocking and no connections are present to be accepted.");
			break ;
		case (EBADF):
			fatal("Socket FD is not an open file descriptor.");
			break ;
		case (ECONNABORTED):
			fatal("A connection has been aborted.");
			break ;
		case (EFAULT):
			fatal("The system call was interrupted by a signal that was caught before a valid connection arrived");
			break ;
		case (EINTR):
			fatal("The socket is marked nonblocking and no connections are present to be accepted.");
			break ;
		case (EINVAL):
			fatal("Socket is not listening for connections, or addrlen is invalid");
			break ;
		case (EMFILE):
			fatal("The per-process limit on the number of open file descriptors has been reached.");
			break ;
		case (ENFILE):
			fatal("The system-wide limit on the total number of open files has been reached.");
			break ;
		case (ENOBUFS):
			fatal("Not enough free memory.");
			break ;
		case (ENOMEM):
			fatal("Not enough free memory.");
			break ;
		case (ENOTSOCK):
			fatal("The file descriptor socket FD does not refer to a socket.");
			break ;
		case (EOPNOTSUPP):
			fatal("The referenced socket is not of type SOCK_STREAM.");
			break ;
		case (EPERM):
			fatal("Firewall rules forbid connection.");
			break ;
		case (EPROTO):
			fatal("Protocol error.");
			break ;
	}
}
