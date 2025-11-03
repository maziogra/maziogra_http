#include <socket/Socket.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef BASESOCKET_H
#define BASESOCKET_H


namespace maziogra_http {
    class BaseSocket : public Socket {
    protected:
        WSADATA wsaData {};
    public:
        BaseSocket() {
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) {
                throw std::runtime_error("WSAStartup failed");
            }
        }

        ~BaseSocket() {
            close();
            WSACleanup();
        }

        bool create(int port) override;
        void close() override;
        std::unique_ptr<Socket> accept() override;
        int send(const char* data, size_t size) override;
        int receive(char* buffer, size_t size) override;

        BaseSocket(const BaseSocket&) = delete;
        BaseSocket& operator=(const BaseSocket&) = delete;
        BaseSocket(BaseSocket&&) noexcept = default;
        BaseSocket& operator=(BaseSocket&&) noexcept = default;
    };

}

#endif
