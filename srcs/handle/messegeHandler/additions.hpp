#pragma once
#include <time.h>
#include <iostream>
#include <map>
#include "../users/user.hpp"

class User;

std::string						getCurrentTime();
time_t							getCurrentTimeForUser();
std::map<int, User>::iterator	getUserByName(std::map<int, User> *clients, std::string name);
std::string						int_to_string(size_t number);
