#pragma once

#include <mutex>
#include <thread>
#include <queue>
#include <map>
#include <functional>
#include <condition_variable>

#define ADDRESS_LEN 256

class ThreadPool {
public:
    explicit ThreadPool(uint32_t subThreadsCount);

    ~ThreadPool();

    void enqueue(int clientSocket, std::function<std::string(const std::string &)> action, const std::string &address);

private:
    uint32_t mWorkersCount;

    bool mIsFinished;

    std::queue<std::tuple<int, std::function<std::string(const std::string &)>, std::string>> mTasks;
    std::vector<std::thread> mWorkers;

    std::mutex mTasksMutex;
    std::mutex mQueriesMutex;

    std::map<std::string, std::string> mPrevQueries;

    std::condition_variable mOnTasksUpdate;

    void initWorkers();

    void joinWorkers();

    void computeTask(int clientSocket, std::function<std::string(const std::string &)> action, const std::string &addressName);
};