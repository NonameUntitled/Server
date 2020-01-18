#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include "ThreadPool.h"

ThreadPool::ThreadPool(uint32_t subThreadsCount)
        : mWorkersCount(subThreadsCount), mIsFinished(false) {
    initWorkers();
}

ThreadPool::~ThreadPool() {
    joinWorkers();
}

void ThreadPool::enqueue(int clientSocket, std::function<std::string(const std::string &)> action, std::string address) {
    {
        std::lock_guard<std::mutex> lock(mTasksMutex);

        if (!mIsFinished) {
            mTasks.emplace(std::make_tuple(clientSocket, action, address));
        }
    }

    mOnTasksUpdate.notify_one();
}

void ThreadPool::initWorkers() {
    mIsFinished = false;
    mWorkers.clear();

    for (size_t i = 0; i < mWorkersCount; ++i) {
        mWorkers.emplace_back([=]() {
            while (true) {
                int clientSocket;
                std::function<std::string(const std::string &)> action;
                std::string address;

                {
                    std::unique_lock<std::mutex> lock(mTasksMutex);

                    if (mIsFinished) {
                        break;
                    }

                    mOnTasksUpdate.wait(lock, [=]() { return mIsFinished || !mTasks.empty(); });

                    if (mIsFinished || mTasks.empty()) {
                        break;
                    }

                    auto task = std::move(mTasks.front());
                    clientSocket = std::get<0>(task);
                    action = std::get<1>(task);
                    address = std::get<2>(task);
                    mTasks.pop();
                }

                computeTask(clientSocket, action, address);
            }
        });
    }
}

void ThreadPool::joinWorkers() {
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        if (mIsFinished) {
            return;
        }

        mIsFinished = true;
    }

    mOnTasksUpdate.notify_all();

    for (auto &worker : mWorkers) {
        worker.join();
    }
}

void ThreadPool::computeTask(int clientSocket, std::function<std::string(const std::string &)> action, const std::string &addressName) {
    {
        std::lock_guard<std::mutex> lock(mQueriesMutex);
        if (mPrevQueries.count(addressName)) {
            std::string response = mPrevQueries[addressName];
            send(clientSocket, response.data(), response.size(), MSG_NOSIGNAL);
            return;
        }
    }

    std::string result = action(addressName);

    {
        std::lock_guard<std::mutex> lock(mQueriesMutex);
        mPrevQueries[addressName] = result;
    }

    send(clientSocket, result.data(), result.size(), MSG_NOSIGNAL);
}