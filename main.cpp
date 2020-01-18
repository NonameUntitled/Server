//#include <iostream>
//#include <algorithm>
//#include <set>
//
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/epoll.h>
//#include <netdb.h>
//#include <arpa/inet.h>
//#include <cstring>
//#include "ThreadPool.h"
//
//#define MAX_EVENTS 64
//#define BUF_SIZE 512
//
//int set_nonblock(int fd) {
//    int flags;
//#if defined(O_NONBLOCK)
//    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
//        flags = 0;
//    }
//    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
//#else
//    flags = 1;
//    return ioclt(fd, FIOBIO, &flags);
//#endif
//}
//
//int main(int argc, char *argv[]) {
////    int port;
////    std::cin >> port;
////    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
////
////    struct sockaddr_in sa;
////    sa.sin_family = AF_INET;
////    sa.sin_port = htons(port);
////    sa.sin_addr.s_addr = htonl(INADDR_ANY);
////    bind(serverSocket, (struct sockaddr *)&sa, sizeof(sa));
////
////    set_nonblock(serverSocket);
////
////    listen(serverSocket, SOMAXCONN);
////
////    int EPoll = epoll_create1(0);
////
////    struct epoll_event Event{};
////    Event.data.fd = serverSocket;
////    Event.events = EPOLLIN;
////    epoll_ctl(EPoll, EPOLL_CTL_ADD, serverSocket, &Event);
//
//    ThreadPool threadPool = ThreadPool(3);
//
//    for (int i = 0; i < 100; ++i) {
//        threadPool.enqueue("google.com");
//    }
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//}