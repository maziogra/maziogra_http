#pragma once

#include <socket/Socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace maziogra_http {

    class SecureSocket : public Socket {
    public:
        SecureSocket();
        ~SecureSocket() {
            close();
        };

        bool create(int port) override;
        bool create(int port,
                    const char* certFile,
                    const char* keyFile);

        void close() override;
        std::unique_ptr<Socket> accept() override;

        ssize_t send(const char* data, size_t size) override;
        ssize_t receive(char* buffer, size_t size) override;

        SecureSocket(const SecureSocket&) = delete;
        SecureSocket& operator=(const SecureSocket&) = delete;
        SecureSocket(SecureSocket&&) noexcept = default;
        SecureSocket& operator=(SecureSocket&&) noexcept = default;

    private:
        SSL_CTX* ctx = nullptr;
        SSL* ssl = nullptr;
        bool owns_ctx = false;

    };

}