#pragma once

#include "support.hpp"
#include <map>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TCP 6 //From 'man protocols' -  /etc/protocols

void	listen_messages(const int port, const std::string pass);
