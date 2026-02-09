#include <socket/SecureSocket.h>
#include <socket/BaseSocket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <socket/Socket.h>
#include <memory>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace maziogra_http {
    static bool ssl_initialized = false;

    SecureSocket::SecureSocket() {
        if (!ssl_initialized) {
            ssl_initialized = true;
            SSL_library_init();
            OpenSSL_add_all_algorithms();
            SSL_load_error_strings();
        }
    }

    void SecureSocket::close() {
        if (ssl) {
            SSL_shutdown(ssl);
            SSL_free(ssl);
            ssl = nullptr;
        }

        if (sock != -1) {
            ::close(sock);
            sock = -1;
        }

        if (ctx && owns_ctx) {
            SSL_CTX_free(ctx);
        }

        ctx = nullptr;
    }

    bool SecureSocket::create(int port) {
        return false;
    }

    bool SecureSocket::create(int port, const char *certFile, const char *keyFile) {
        ctx = SSL_CTX_new(TLS_server_method());
        owns_ctx = true;
        if (!ctx) {
            ERR_print_errors_fp(stderr);
            return false;
        }

        if (SSL_CTX_use_certificate_file(ctx, certFile, SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return false;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx, keyFile, SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return false;
        }

        if (!SSL_CTX_check_private_key(ctx)) {
            ERR_print_errors_fp(stderr);
            return false;
        }

        sock = ::socket(AF_INET, SOCK_STREAM, 0);

        if (sock == -1) return false;

        sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;

        if (::bind(sock, (sockaddr *) &sockAddr, sizeof(sockAddr)) < 0) {
            close();
            return false;
        }

        if (::listen(sock, SOMAXCONN) < 0) {
            close();
            return false;
        }

        return true;
    }

    std::unique_ptr<Socket> SecureSocket::accept() {
        int clientSocket = ::accept(sock, nullptr, nullptr);
        if (clientSocket == -1) return nullptr;

        auto client = std::make_unique<SecureSocket>();
        client->sock = clientSocket;
        client->ctx = ctx;
        client->owns_ctx = false;

        client->ssl = SSL_new(ctx);
        if (!client->ssl) {
            ::close(clientSocket);
            return nullptr;
        }

        SSL_set_fd(client->ssl, clientSocket);

        if (SSL_accept(client->ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            ::close(clientSocket);
            client->ssl = nullptr;
            return nullptr;
        }

        return client;
    }

    ssize_t SecureSocket::send(const char *data, size_t size) {
        if (!ssl) return -1;

        ssize_t totalSent = 0;

        while (totalSent < size) {
            ssize_t n = SSL_write(ssl, data + totalSent, static_cast<int>(size - totalSent));
            if (n > 0) {
                totalSent += n;
                continue;
            }

            int err = SSL_get_error(ssl, n);
            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                continue;
            } else if (err == SSL_ERROR_ZERO_RETURN) {
                break;
            } else {
                return -1;
            }
        }

        return totalSent;
    }

    ssize_t SecureSocket::receive(char *buffer, size_t size) {
        if (!ssl) return -1;
        return SSL_read(ssl, buffer, (int) (size));
    }
}
