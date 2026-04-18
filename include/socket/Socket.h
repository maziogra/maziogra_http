#pragma once
#include <memory>
#if _WIN32
#include <winsock2.h>
#endif

namespace maziogra_http {
    class Socket {
    public:
        Socket() = default;
        ~Socket() = default;

        virtual bool create(int port) = 0;
        virtual void close() = 0;
        virtual std::unique_ptr<Socket> accept() = 0;
        virtual std::ptrdiff_t send(const char* data, size_t size) = 0;
        virtual std::ptrdiff_t receive(char* buffer, size_t size) = 0;
        int getSocket() const {
            return sock;
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;
        Socket(Socket&&) noexcept = default;
        Socket& operator=(Socket&&) noexcept = default;

    protected:
#if _WIN32
		SOCKET sock = INVALID_SOCKET;
#else
        int sock = -1;
#endif
        
    };

}