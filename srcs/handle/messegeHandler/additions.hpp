#include <time.h>
#include <iostream>
#include <map>
#include "../users/user.hpp"

const std::string	getCurrentTime();
std::map<int, User>::iterator getUserByName(std::map<int, User> *clients, std::string name);