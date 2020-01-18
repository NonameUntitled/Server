#include "Epoll.h"

#include <utility>
#include "Utils.h"

Epoll::Epoll(uint32_t workersCount) : mEpollFd(-1), mThreadPool(workersCount) {
    int result = epoll_create1(0);
    if (result < 0) {
        throwException("Creating epoll error");
    }

    mEpollFd = result;
}

void Epoll::addServer(int serverFd, std::function<std::string(const std::string &)> serverFunction) {
    struct epoll_event Event{};
    Event.data.fd = serverFd;
    Event.events = EPOLLIN;

    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, serverFd, &Event) == -1) {
        throwException("Add server epoll descriptor");
    }

    mFromServerToProcess[serverFd] = std::move(serverFunction);
    mServers.insert(serverFd);
}

void Epoll::run() {
    while (true) {
        struct epoll_event Events[MAX_EVENTS];
        int queriesCount = epoll_wait(mEpollFd, Events, MAX_EVENTS, -1);

        if (queriesCount < 0) {
            throwException("Epoll wait error");
        }

        for (int i = 0; i < queriesCount; ++i) {
            try {
                if (mServers.count(Events[i].data.fd)) {
                    int clientSocket = accept(Events[i].data.fd, nullptr, nullptr);

                    set_nonblock(clientSocket);

                    struct epoll_event clientEvent{};
                    clientEvent.data.fd = clientSocket;
                    clientEvent.events = EPOLLIN;

                    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1) {
                        throwException("Create new epoll connection");
                    } else {
                        mFromServerToProcess[clientSocket] = mFromServerToProcess[Events[i].data.fd];
                    }
                } else {
                    char buffer[BUF_SIZE] = {};
                    int recvRes = recv(Events[i].data.fd, buffer, BUF_SIZE, MSG_NOSIGNAL);
                    if (recvRes < 0) {
                        throwException("Read error");
                    }
                    if (recvRes == 0 && errno != EAGAIN) {
                        shutdown(Events[i].data.fd, SHUT_RDWR);
                        close(Events[i].data.fd);
                    } else {
                        buffer[recvRes - 2] = '\0';
                        mThreadPool.enqueue(Events[i].data.fd, mFromServerToProcess[Events[i].data.fd], std::string(buffer));
                    }
                }
            } catch (std::exception &exception) {
                std::clog << exception.what();
            } catch (...) {
                std::clog << "Unknown error occurred!" << '\n';
            }
        }
    }
}
