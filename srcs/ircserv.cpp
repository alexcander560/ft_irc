#include "listen/listen.hpp"

void	stop(std::string str)
{
	std::cerr << str << std::endl;
	exit(EXIT_FAILURE);
}

int	main(int args_count, char **args)
{
	int	port;

	if (args_count != 3)
		stop("It's few to params");
	port = std::atoi(args[1]);
	if (port < 1024 || port > 49151)
		stop("Wrong port");
	if (strlen(args[2]) == 0)
		stop("Empty password");
	listen_messages(port, std::string(args[2]));
}
