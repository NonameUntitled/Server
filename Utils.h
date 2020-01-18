#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <iostream>
#include <algorithm>
#include <set>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

int set_nonblock(int fd);

void throwException(const std::string &errorDescr);

#endif