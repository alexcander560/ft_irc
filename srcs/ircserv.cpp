#include "listen/listen.hpp"

void  stop(std::string str)
{
	std::cerr << str << std::endl;
	exit(EXIT_FAILURE);
}

int  main(int args_count, char **args)
{
	int  port;

	if (args_count != 3)
		stop("./ircserv [host:port_network:password_network] <port> <password>");
	port = std::atoi(args[1]);
	if (port < 1024 || port > 49151)
		stop("It's wrong port. Correct range is from 1024 to 49151");
	if (strlen(args[2]) == 0)
		stop("Password cannot be empty");
	listen_messages(port, std::string(args[2]));
}