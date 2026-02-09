#ifndef SOCKET_H
#define SOCKET_H

#include <memory>
namespace maziogra_http {
    class Socket {
    public:
        Socket() = default;
        ~Socket() = default;

        virtual bool create(int port) = 0;
        virtual void close() = 0;
        virtual std::unique_ptr<Socket> accept() = 0;
        virtual ssize_t send(const char* data, size_t size) = 0;
        virtual ssize_t receive(char* buffer, size_t size) = 0;
        int getSocket() const {
            return sock;
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;
        Socket(Socket&&) noexcept = default;
        Socket& operator=(Socket&&) noexcept = default;

    protected:
        int sock = -1;
    };

}

#endif
