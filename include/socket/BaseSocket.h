#pragma once
#include <socket/Socket.h>

namespace maziogra_http {
    class BaseSocket : public Socket {
    public:
        BaseSocket() = default;

        ~BaseSocket() {
            close();
        }

        bool create(int port) override;
        void close() override;
        std::unique_ptr<Socket> accept() override;
        std::ptrdiff_t send(const char* data, size_t size) override;
        std::ptrdiff_t receive(char* buffer, size_t size) override;

        BaseSocket(const BaseSocket&) = delete;
        BaseSocket& operator=(const BaseSocket&) = delete;
        BaseSocket(BaseSocket&&) noexcept = default;
        BaseSocket& operator=(BaseSocket&&) noexcept = default;
    };

}