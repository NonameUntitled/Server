#include "Server.h"
#include "Epoll.h"
#include "Utils.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        stringstream message;
        message << "Invalid usage! Usage: " << argv[0] << " <server_address> <server_port>";
        return -1;
    }

    Epoll epoll = Epoll(3);
    Server getAddrServer = Server(argv[1], argv[2]);

    try {
        getAddrServer.configureServer();

        epoll.addServer(getAddrServer.getFd(), [](const std::string &message) {
            struct addrinfo hints{}, *res;
            char addressString[ADDRESS_LEN];
            void *ptr;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags |= AI_CANONNAME;

            std::stringstream response;
            try {
                if (getaddrinfo(message.data(), nullptr, &hints, &res) != 0) {
                    throwException("Incorrect address");
                }

                response << "Host: " << message << '\n';

                for (auto resPointer = res; resPointer != nullptr; resPointer = resPointer->ai_next) {
                    inet_ntop(resPointer->ai_family, resPointer->ai_addr->sa_data, addressString, ADDRESS_LEN);

                    switch (resPointer->ai_family) {
                        case AF_INET:
                            ptr = &((struct sockaddr_in *) resPointer->ai_addr)->sin_addr;
                            break;
                        case AF_INET6:
                            ptr = &((struct sockaddr_in6 *) resPointer->ai_addr)->sin6_addr;
                            break;
                        default:
                            throw std::invalid_argument("Unknown family type!");
                    }

                    inet_ntop(resPointer->ai_family, ptr, addressString, ADDRESS_LEN);
                    response << "IPv" << (resPointer->ai_family == PF_INET6 ? 6 : 4) << " address: " << addressString
                             << '\n';
                }
            }
            catch (std::exception &exception) {
                response.clear();
                response << exception.what();
            }

            return response.str();
        });
    } catch (std::exception &exception) {
        std::cerr << exception.what();
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << '\n';
        return -1;
    }

    epoll.run();

    return 0;
}