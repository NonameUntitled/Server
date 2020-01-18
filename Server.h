#pragma once

#include "ThreadPool.h"

#define MIN_PORT_VALUE 1
#define MAX_PORT_VALUE 65535


class Server {
public:
    Server(const char *server, const char *port);

    void configureServer();

    int getFd();

private:
    void createSocket();

    void bindSocket();

    void startListen();

//    void handleConnections();

    const char *mAddress;
    int mPort;
    int mServerFd;
};