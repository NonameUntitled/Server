cmake_minimum_required(VERSION 3.15)
project(Server)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_CXX_STANDARD 17)

add_executable(Server Server.cpp Server.h serverMain.cpp main.cpp ThreadPool.cpp ThreadPool.h Epoll.cpp Epoll.h Utils.cpp Utils.h)