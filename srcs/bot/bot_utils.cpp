#include "bot.hpp"

/* Вспомогательный файл для бота */
/* Получение команды и информации из принимаемой строки */

/* Из 'GETIMAGE прекрасный котик' возвращает 'GETIMAGE' */
string	get_command_by_msg(std::string &msg) {
	size_t	position = 0;
	string	command(msg);

	while (position < command.size() && command[position] == ' ')
		position++;
	command.erase(0, position);
	while (position < command.size() && command[position] != ' ')
		position++;
	command.erase(position, command.size() - position);
	return (command);
}

/* Из 'GETIMAGE прекрасный котик' возвращает 'прекрасный котик' */
string	get_data_by_msg(std::string &msg) {
	size_t	position = 0;
	string	command(msg);

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
