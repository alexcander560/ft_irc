#include "bot.hpp"

string	get_command_by_msg(std::string &msg)
{
	int	position = 0;
	std::string	command(msg);

	while (position < command.size() && command[position] == ' ')
		position++;
	command.erase(0, position);
	while (position < command.size() && command[position] != ' ')
		position++;
	command.erase(position, command.size() - position);
	return (command);
}

string	get_data_by_msg(std::string &msg)
{
	int	position = 0;
	std::string	command(msg);

	while (position < command.size() && command[position] == ' ')
		position++;
	command.erase(0, position);
	position = 0;
	while (position < command.size() && command[position] != ' ')
		position++;
	command.erase(0, position);
	position = 0;
	while (position < command.size() && command[position] == ' ')
		position++;
	command.erase(0, position);
	return (command);
}
