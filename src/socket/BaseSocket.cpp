#include <iostream>
#include <socket/BaseSocket.h>
#include <winsock2.h>
#include <memory>
#include <stdexcept>

namespace maziogra_http {

    bool BaseSocket::create(const int port) {
        sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) return false;

        sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;

        if (::bind(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
            close();
            return false;
        }

        if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
            close();
            return false;
        }

        return true;
    }


    void BaseSocket::close() {
        if (sock != INVALID_SOCKET) {
            ::closesocket(sock);
            sock = INVALID_SOCKET;
        }
    }

    std::unique_ptr<Socket> BaseSocket::accept() {
        SOCKET clientSocket = ::accept(sock, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) return nullptr;

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
