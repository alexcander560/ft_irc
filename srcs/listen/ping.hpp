#pragma once

#include <map>
#include "listen.hpp"

bool	check_time(std::map<int, User> *clients_map, std::string pass, std::map<int, string> &clients, fd_set &fds, map<int, string> &ip, std::vector<Channel> *channel);
