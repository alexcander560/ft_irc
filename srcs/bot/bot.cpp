#include "bot.hpp"

#define IS_COLOR_SET 0 //Set on TRUE for console; set on FALSE for Adium
#define PREFIX ":bot!DragonsCHAT@127.0.0.1 PRIVMSG "

int										_id;
string									_name;
vector< std::pair<int, string> >	*_message;

void	setColor(string &lines, string color = DEFAULT) {
	if (IS_COLOR_SET)
		lines.append(color);
}

void	addLine(string line, string color = "") {
	string	def_color = DEFAULT;

	if (!IS_COLOR_SET)
	{
		def_color = "";
		color = "";
	}
	_message->push_back(std::make_pair(_id, PREFIX + _name + " :" + color + line + def_color + "\n"));
}

/* Главный файл для бота */
/* P.S. Дракончики */

/* Команда HELP */
void	help_command(void) {
	debug("[help_command] Command HELP for bot was called");
	addLine("Commands for you:", COLOR_FOR_BOT);
	addLine("\tHELP - show this information", COLOR_FOR_BOT);
	addLine("\tGETHASH [your text] - encrypt your data for high level security", COLOR_FOR_BOT);
	addLine("\tGETIMAGE [phrase] - get link to the image by phrase (can looking for beautiful cats)", COLOR_FOR_BOT);
}

/* Команда GETHASH */
void	get_hash_command(string text) {
	debug("[get_hash_command] Command GETHASH for bot was called");
	if (text.empty()) {
		addLine("Oooops. Is empty!? I can not!!!", RED);
		return ;
	}
	addLine("Your hash: " + string(crypt(text.c_str(), "openKey42")), COLOR_FOR_BOT);
}

/* Команда GETIMAGE */
void	get_image_command(string text) {
	string	link;

	debug("[get_image_command] Command GETIMAGE for bot was called");
	if (text.empty()) {
		addLine("Oooops. Is empty!? I can not!!!", RED);
		return ;
	}
	try {
		link = get_link(text.c_str());
	}
	catch (string e) {
		warning("[get_image_command] Cannot get image by name. Captcha in Yandex?");
		warning(e.c_str());
		addLine("Oooops. Captcha!? I can not!!!", RED);
		return ;
	}
	addLine("Your image here: " + link, COLOR_FOR_BOT);
}

/* Неизвестная команда */
void	not_found_command(void) {
	warning(YELLOW"[not_found_command] Unknown command was called for bot." DEFAULT);
	addLine("¯\\_(ツ)_/¯\n", RED);
	addLine("Use HELP command for information about my beautiful life", RED);
}

/* Основная функция для бота. Обрабатывает строку, возвращает результат для отправки */
void	handle_command(string line, int id, string name, std::vector< std::pair<int, string> > *message) {
	string	command = get_command_by_msg(line);
	string	text = get_data_by_msg(line);

	_id = id;
	_name = name;
	_message = message;
	if (command == COMMAND_HELP)
		help_command();
	else if (command == COMMAND_GETHASH)
		get_hash_command(text);
	else if (command == COMMAND_GETIMAGE)
		get_image_command(text);
	else
		not_found_command();
}
