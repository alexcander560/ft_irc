#include "listen.hpp"
#include "../general.hpp"

/* Прослушивание порта и передача итогового сообщения в обработчик */
/* Состоит из кучи проверок. Не пугаться */
/* P.S. Драконы */

/* Создание сокета под IPv4, TCP, и проверка функции на ошибки */
static int	create_socket()
{
	int	socket_fd;


	socket_fd = socket(PF_INET, SOCK_STREAM, TCP);
	if (socket_fd != -1)
		return (socket_fd);
	fatal(std::strerror(errno));
	return (-1);
}

/* Настройка сокета, проверка */
static void	set_socket_options(const int socket_fd)
{
	int		set_options;
	int		enable_operation = true;

	set_options = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
							 static_cast<void *>(&enable_operation), sizeof(enable_operation));
	if (set_options != -1)
		return ;
	fatal(std::strerror(errno));
}

/* Выдача сокету "имя" в виде адреса и порта */
static void	bind_socket(const int socket_fd, const int port)
{
	int	bind_value;
	struct sockaddr_in address; //For IPv4 instead IPv6 (sockaddr_in6);

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	bind_value = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	if (bind_value != -1)
		return ;
	fatal(std::strerror(errno));
}

/* Установка хука на активность сокета */
static void	listen_socket(const int socket_fd)
{
	int	listen_value;

	listen_value = listen(socket_fd, 42);
	if (listen_value != -1)
		return ;
	fatal(std::strerror(errno));
}

/* Слушаем отдельного клиента - отдельную машину, если угодно */
void	listen_clients(const int socket_fd)
{
	int						connection_fd;
	fd_set					fds, read_fds;
	map<int, std::string>	clients;
	map<int, User>			clients_map;		//структура данных со всеми пользователями
	

	FD_ZERO(&fds);
	FD_SET(socket_fd, &fds);

	debug("[listen_clients] Listening clients");
	
	while (true)
	{
		FD_COPY(&fds, &read_fds);
		handle_select( select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) );
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				if (i == socket_fd) //Новое подключение
				{
					connection_fd = accept(socket_fd, NULL, NULL);
					handle_accept(connection_fd);
					clients.insert(std::pair<int, std::string>(connection_fd, ""));
					FD_SET(connection_fd, &fds);

					std::cout	<< "DEBUG: [listen_clients] New connection with ID "
									<< connection_fd << " was add" << std::endl;
				}
				else //Старое подключение - обработка
				{
					char	buffer;
					int	bytes = recv(i, &buffer, 1, 0);
					if (bytes != 1) //Пользователь отключился. Удаляем из MAP
					{
						std::cout	<< "DEBUG: [listen_clients] Client with ID "
										<< i << " was disconnected. Active users count: "
										<< clients.size() - 1 << std::endl;

						close(i);
						FD_CLR(i, &fds);
						clients.erase(i);
					}
					else //Пользователь ввел данные, обрабатываем
					{
						if (buffer != 10) //Если введен любой символ, кроме ENTER в консоли
							add_character_by_id(i, buffer, clients);
						else //Если символ - конец строки (ENTER в консоли)
						{
							std::string line = add_character_by_id(i, '\0', clients);

							line = line.substr(0, line.size() - 1);
							printf("ID [%d]: %s\n", i, line.c_str()); // Для команды: ВЫВОД НА СЕРВЕР

							try
							{
								send_message(handle_message(line, i, &clients_map, "123")); // Для команды: ОТПРАВКА ПОЛЬЗОВАТЕЛЮ СООБЩЕНИЯ
							}
							catch (std::string e)
							{
								//TODO: FOR ALEX		Show message?		FOR ALEX
							}
							clear_by_id(i, clients);
						}
					}
				}
			}
		}
	}
}

/* Главная функция для прослушивания. Запускает все второстепенные, является главным узлом целого блока */
void	listen_messages(const int port)
{
	int	socket_fd;

	socket_fd = create_socket();
	debug("[listen_messages] create_socket() successful");
	set_socket_options(socket_fd);
	debug("[listen_messages] set_socket_options() successful");
	bind_socket(socket_fd, port);
	debug("[listen_messages] bind_socket() successful");
	listen_socket(socket_fd);
	debug("[listen_messages] listen_socket() successful");
	listen_clients(socket_fd);
}
