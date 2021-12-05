#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "bot/bot.hpp"

using	std::cout;
using	std::endl;
using	std::string;
using	std::pair;
using	std::make_pair;
using	std::map;
using	std::vector;
using 	std::set;

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

#include "handle/users/user.hpp"
// #include "handle/messegeHandler/messegeHandler.hpp"

class User;

void	message(string line);
void	warning(string line);
void	fatal(string line);
void	debug(string line);

vector< pair<int, string> >	handle_message(string str_message, int id, map<int, User> *clients_map, string pass, map<int, string> &clients, fd_set &fds, string ip);
