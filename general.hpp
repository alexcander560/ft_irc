#pragma once

#include <iostream>

#define DEBUG 1 //Дебажим? Для более обширных логов в FD = 1 (console write FD)

void	message(std::string line);
void	warning(std::string line);
void	fatal(std::string line);

void	debug(std::string line);