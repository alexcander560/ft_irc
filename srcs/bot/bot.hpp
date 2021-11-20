#pragma once

#include <iostream>
#include <string>
#include "../general.hpp"
#include "image/image.hpp"
#include <unistd.h>

using std::string;

#define COMMAND_HELP "HELP"
#define COMMAND_GETHASH "GETHASH"
#define COMMAND_GETIMAGE "GETIMAGE"

#define COLOR_FOR_BOT GREEN

/* Вспомогательные функции. Пожалуйста, не надо нас ломать */
string	get_command_by_msg(std::string &msg);
string	get_data_by_msg(std::string &msg);
/* Вспомогательные функции. Пожалуйста, не надо нас ломать */

string	handle_command(string line); /* Основаная команда для обработки. Отправляется params[2], например: 'GETIMAGE beautiful cat' */
