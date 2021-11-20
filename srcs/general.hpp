#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "bot/bot.hpp"

using	std::cout;
using	std::endl;
using	std::string;
using	std::pair;
using	std::make_pair;
using	std::map;
using	std::vector;

# define DEFAULT	"\033[0m"
# define BOLD		"\033[1m"
# define UNDERLINE	"\033[4m"
# define REVERS		"\033[7m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define AQUA		"\033[36m"

#define DEBUG 1 //Дебажим? Для более обширных логов в FD = 1 (console write FD)
#define DEBUG_LOG 0

#include "handle/users/user.hpp"

class User;

void	message(std::string line);
void	warning(std::string line);
void	fatal(std::string line);

void	debug(std::string line);

pair<int, string>	handle_message(string str_message, int id, map<int, User> *clients_map, string pass);
