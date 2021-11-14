#include "message_listen.hpp"
#include "general.hpp"

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
	switch (errno)
	{
		case EACCES:
			fatal("Permission to create a socket of the specified type"
						 "and/or protocol is denied.");
			break ;
		case EAFNOSUPPORT:
			fatal("Permission to create a socket of the specified type"
						 "and/or protocol is denied.");
			break ;
		case EMFILE:
			fatal("The per-process descriptor table is full.");
			break ;
		case ENFILE:
			fatal("The system file table is full.");
			break ;
		case ENOBUFS:
			fatal("Insufficient buffer space is available.  The socket   cannot be created until sufficient resources are   freed.");
			break ;
		case ENOMEM:
			fatal("Insufficient memory was available to fulfill the   request.");
			break ;
		case EPROTONOSUPPORT:
			fatal("The protocol type or the specified protocol is not   supported within this domain.");
			break ;
		case EPROTOTYPE:
			fatal("The socket type is not supported by the protocol.");
			break ;
		default:
			fatal("Unknown error.");
	}
	return (-1);
}

static void	set_socket_options(const int socket_fd)
{
	int		set_options;
	int		enable_operation = true;

	set_options = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
							 static_cast<void *>(&enable_operation), sizeof(enable_operation));
	if (set_options != -1)
		return ;
	switch (errno)
	{
		case EDOM:
			fatal("The argument option_value is out of bounds.");
			break ;
		case EISCONN:
			fatal("Socket is already connected and a specified option   cannot be set while this is the case.");
			break ;
		case EINVAL:
			fatal("The socket has been shut down.");
			break ;
		default:
			fatal("Unknown error.");
	}
}

static void	bind_socket(const int socket_fd, const int port)
{
	int	bind_value;
	struct sockaddr_in address; //For IPv4 instead IPv6 (sockaddr_in6);

	address.sin_family = AF_INET;
	address.sin_port = port;
	address.sin_addr.s_addr = INADDR_ANY;

	bind_value = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	if (bind_value != -1)
		return ;
	switch (errno)
	{
		case EACCES:
			fatal("The address is protected, and the user is not the   superuser.");
			break ;
		case EADDRINUSE:
			fatal("The port number was specified as zero in the socket address structure, but, upon attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.");
			break ;
		case EBADF:
			fatal("Socket FD is not a valid file descriptor.");
			break ;
		case EINVAL:
			fatal("The socket is already bound to an address.");
			break ;
		case EADDRNOTAVAIL:
			fatal("A nonexistent interface was requested or the requested address was not local.");
			break ;
		case EFAULT:
			fatal("Address points outside the user's accessible address space.");
			break ;
		case ELOOP:
			fatal("Too many symbolic links were encountered in resolving address.");
			break ;
		case ENAMETOOLONG:
			fatal("Address is too long.");
			break ;
		case ENOENT:
			fatal("A component in the directory prefix of the socket pathname does not exist.");
			break ;
		case ENOMEM:
			fatal("Insufficient kernel memory was available.");
			break ;
		case ENOTDIR:
			fatal("A component of the path prefix is not a directory.");
			break ;
		case EROFS:
			fatal("The socket inode would reside on a read-only filesystem.");
			break ;
		default:
			fatal("Unknown error.");
			break ;
	}
}

void	listen_messages(const int port)
{
	int	socket_fd;

	socket_fd = create_socket();
	set_socket_options(socket_fd);
	bind_socket(socket_fd, port);
}
