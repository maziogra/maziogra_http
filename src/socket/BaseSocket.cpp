
#include <socket/BaseSocket.h>
#include <sys/types.h>
#include <socket/Socket.h>
#include <memory>

#if _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif
#include <iostream>

namespace maziogra_http {
    bool BaseSocket::create(const int port) {
        sock = ::socket(AF_INET, SOCK_STREAM, 0);
#if _WIN32
        if (sock == INVALID_SOCKET) return false;
#else
		if (socket == -1) return false;
#endif

        sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;
#if _WIN32
        if (::bind(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
#else
        if (::bind(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {
#endif
            close();
            return false;
        }

#if _WIN32
        if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
#else
        if (::listen(sock, SOMAXCONN) < 0) {
#endif
            close();
            return false;
        }

        return true;
    }


    void BaseSocket::close() {
        if (sock != -1) {
#if _WIN32
			::closesocket(sock);
#else
            ::close(sock);
#endif
            sock = -1;
        }
    }

    std::unique_ptr<Socket> BaseSocket::accept() {
#if _WIN32
        SOCKET clientSocket;
#else
        int clientSocket;
#endif
        clientSocket = ::accept(sock, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            return nullptr;
        }

        auto client = std::make_unique<BaseSocket>();
        client->sock = clientSocket;
        return client;
    }


    std::ptrdiff_t BaseSocket::send(const char* data, size_t size) {
        std::ptrdiff_t totalSent = 0;

        while (totalSent < size) {
            std::ptrdiff_t sent = ::send(sock, data+totalSent, size-totalSent, 0);
            if (sent < 0) {
                return -1;
            }
            totalSent += sent;
        }

        return totalSent;
    }

    std::ptrdiff_t BaseSocket::receive(char* buffer, size_t size) {
        while (true) {
            std::ptrdiff_t n = ::recv(sock, buffer, size, 0);

            if (n >= 0)
                return n;

            if (errno == EINTR)
                continue;

            return -1;
        }
    }


}
