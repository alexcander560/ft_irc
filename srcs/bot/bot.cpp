#include "bot.hpp"

#define COLOR_FOR_BOT GREEN

string	help_command(void)
{
	std::string	help;

	debug("[help_command] Command HELP for bot was called");
	help.append(COLOR_FOR_BOT); // Установка цвета
	help.append("Commands for you:\n");
	help.append("\tHELP - show this information\n");
	help.append("\tENCRYPT [your text] - encrypt your data for high level security\n");
	help.append("\tDECRYPT [your text] - decrypt your data that has been encrypted\n");
	help.append("\tGETIMAGE [phrase] - get link to the image by phrase (can looking for beautiful cats)\n");
	help.append(DEFAULT); // Установка цвета
	return (help);
}

string	get_hash_command(std::string text)
{
	std::string	hash;

	debug("[get_hash_command] Command GETHASH for bot was called");
	if (text.empty())
	{
		hash.append(RED"Oooops. Is empty!? I can not!!!\n"DEFAULT);
		goto done;
	}
	hash.append(COLOR_FOR_BOT); // Установка цвета
	hash.append("Your hash: ");
	hash.append(crypt(text.c_str(), "openKey42"));
	hash.append("\n");
	hash.append(DEFAULT); // Установка цвета
done:
	return (hash);
}

string	get_image_command(std::string text)
{
	std::string	image;
	std::string link;

	debug("[get_image_command] Command GETIMAGE for bot was called");
	if (text.empty())
	{
		image.append(RED"Oooops. Is empty!? I can not!!!\n"DEFAULT);
		goto done;
	}
	try
	{
		link = get_link(text.c_str());
	}
	catch (std::string e)
	{
		warning("[get_image_command] Cannot get image by name. Captcha in Yandex?");
		warning(e.c_str());
		image.append(RED"Oooops. Captcha!? I can not!!!\n"DEFAULT);
		goto done;
	}
	image.append(COLOR_FOR_BOT); // Установка цвета
	image.append("Your image here: ");
	image.append(link);
	image.append("\n");
	image.append(DEFAULT); // Установка цвета
done:
	return (image);
}

string	not_found_command(void)
{
	std::string	not_found;

	warning(YELLOW"[not_found_command] Unknown command was called for bot."DEFAULT);
	not_found.append(RED); // Установка цвета
	not_found.append("¯\\_(ツ)_/¯\n");
	not_found.append("Use HELP command for information about my beautiful life\n");
	not_found.append(DEFAULT); // Установка цвета
	return (not_found);
}

string	handle_command(string line)
{
	std::string	command = get_command_by_msg(line);
	std::string	text = get_data_by_msg(line);

	if (command == COMMAND_HELP)
		return (help_command());
	else if (command == COMMAND_GETHASH)
		return (get_hash_command(text));
	else if (command == COMMAND_GETIMAGE)
		return (get_image_command(text));
	else
		return (not_found_command());
}
