#pragma once

#include <map>
#include "listen.hpp"
#include "../../general.hpp"

void 	clear_by_id(int id, std::map<int, std::string> &clients);
std::string	&add_character_by_id(int id, int character, std::map<int, std::string> &clients);
void	handle_select(const int select_value);
void	handle_accept(const int select_accept);