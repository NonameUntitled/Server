#ifndef SERVER_EPOLL_H
#define SERVER_EPOLL_H

#define BUF_SIZE 1024
#define MAX_EVENTS 64

#include <set>
#include <map>
#include <functional>
#include "ThreadPool.h"

class Epoll {
public:
    explicit Epoll(uint32_t);

    Epoll(const Epoll &) = delete;

    Epoll(Epoll &&) = delete;

    Epoll &operator=(const Epoll &) = delete;

    Epoll &operator=(Epoll &&) = delete;

    void addServer(int serverFd, std::function<std::string (const std::string &)> serverFunction);

    void run();

private:
    int mEpollFd;

    std::set<int> mServers;

    std::map<int, std::function<std::string (const std::string &)>> mFromServerToProcess;

    ThreadPool mThreadPool;
};

#endif