#include <socket/BaseSocket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <socket/Socket.h>
#include <memory>
#include <unistd.h>

namespace maziogra_http {
    bool BaseSocket::create(const int port) {
        sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) return false;

        sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;

        if (::bind(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {
            close();
            return false;
        }

        if (::listen(sock, SOMAXCONN) < 0) {
            close();
            return false;
        }

        return true;
    }


    void BaseSocket::close() {
        if (sock != -1) {
            ::close(sock);
            sock = -1;
        }
    }

    std::unique_ptr<Socket> BaseSocket::accept() {
        int clientSocket = ::accept(sock, nullptr, nullptr);
        if (clientSocket == -1) return nullptr;

        auto client = std::make_unique<BaseSocket>();
        client->sock = clientSocket;
        return client;
    }


    int BaseSocket::send(const char* data, size_t size) {
        return ::send(sock, data, size, 0);
    }

    int BaseSocket::receive(char* buffer, size_t size) {
        return ::recv(sock, buffer, size, 0);
    }

}
