#pragma once

#include <iostream>
#include <string>
#include "../../general.hpp"
#include "image/image.hpp"
#include <unistd.h>

using std::string;

#define COMMAND_HELP "HELP"
#define COMMAND_GETHASH "GETHASH"
#define COMMAND_GETIMAGE "GETIMAGE"

#define ENCRYPT_KEY 42 //From 1 to 254

string	get_command_by_msg(std::string &msg);
string	get_data_by_msg(std::string &msg);
string	handle_command(string line);
