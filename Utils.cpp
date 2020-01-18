#include <cstring>
#include <sstream>
#include <stdexcept>

#include "Utils.h"

int set_nonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioclt(fd, FIOBIO, &flags);
#endif
}


void throwException(const std::string &errorDescr) {
    std::stringstream ss;
    ss << errorDescr << " has been occurred!" << '\n';
    throw std::runtime_error(ss.str());
}