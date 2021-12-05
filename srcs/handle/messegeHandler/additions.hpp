#include <time.h>
#include <iostream>
#include <map>
#include "../users/user.hpp"

const std::string	getCurrentTime();
const time_t		getCurrentTimeForUser();
std::map<int, User>::iterator getUserByName(std::map<int, User> *clients, std::string name);