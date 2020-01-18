#include "Server.h"
#include "Utils.h"

Server::Server(const char *server, const char *port)
        : mAddress(server), mServerFd(-1) {
    mPort = std::stoi(port);

    if (mPort < MIN_PORT_VALUE || mPort > MAX_PORT_VALUE) {
        std::stringstream message;
        message << "Invalid port value! Port value must be an integer in range [" << std::to_string(MIN_PORT_VALUE)
                << ".." << std::to_string(MAX_PORT_VALUE) << "]";
        throw std::invalid_argument(message.str());
    }
}

void Server::configureServer() {
    createSocket();
    bindSocket();
    startListen();
}

void Server::createSocket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        throwException("Socket creation error");
    }

    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        throwException("setsockopt(SO_REUSEADDR) error");
    }

    mServerFd = fd;
}

void Server::bindSocket() {
    struct sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(mPort);
    inet_pton(AF_INET, mAddress, &sa.sin_addr);

    if (bind(mServerFd, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        throwException("Binding creation error");
    }

    set_nonblock(mServerFd);
}

void Server::startListen() {
    if (listen(mServerFd, SOMAXCONN) == -1) {
        throwException("Start listening error");
    }
}

int Server::getFd() {
    return mServerFd;
}