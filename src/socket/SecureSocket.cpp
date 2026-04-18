#include <socket/SecureSocket.h>
#include <socket/BaseSocket.h>
#include <sys/types.h>
#include <socket/Socket.h>
#include <memory>
#include <openssl/ssl.h>
#include <openssl/err.h>

#if _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

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
#if _WIN32
			::closesocket(sock);
#else
            ::close(sock);
#endif
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

#if _WIN32
		if (sock == INVALID_SOCKET) return false;
#else
        if (sock == -1) return false;
#endif
        sockaddr_in sockAddr{};
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;

#if _WIN32
		if (::bind(sock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
#else
        if (::bind(sock, (sockaddr *) &sockAddr, sizeof(sockAddr)) < 0) {
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

    std::unique_ptr<Socket> SecureSocket::accept() {
#if _WIN32
		SOCKET clientSocket = INVALID_SOCKET;
#else
		int clientSocket = -1;
#endif
        
        clientSocket = ::accept(sock, nullptr, nullptr);
#if _WIN32
		if clientSocket == INVALID_SOCKET) return nullptr;
#else
        if (clientSocket == -1) return nullptr;
#endif

        auto client = std::make_unique<SecureSocket>();
        client->sock = clientSocket;
        client->ctx = ctx;
        client->owns_ctx = false;

        client->ssl = SSL_new(ctx);
        if (!client->ssl) {
#if _WIN32
            ::closesocket(clientSocket);
#else
            ::close(clientSocket);
#endif
            return nullptr;
        }

        SSL_set_fd(client->ssl, clientSocket);

        if (SSL_accept(client->ssl) <= 0) {
            ERR_print_errors_fp(stderr);
#if _WIN32
			::closesocket(clientSocket);
#else
            ::close(clientSocket);
#endif
            client->ssl = nullptr;
            return nullptr;
        }

        return client;
    }

    std::ptrdiff_t SecureSocket::send(const char *data, size_t size) {
        if (!ssl) return -1;

        std::ptrdiff_t totalSent = 0;

        while (totalSent < size) {
            std::ptrdiff_t n = SSL_write(ssl, data + totalSent, static_cast<int>(size - totalSent));
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

    std::ptrdiff_t SecureSocket::receive(char* buffer, size_t size) {
        if (!ssl) return -1;

        while (true) {
            int n = SSL_read(ssl, buffer, static_cast<int>(size));

            if (n > 0) {
                return n;
            }

            int err = SSL_get_error(ssl, n);

            switch (err) {
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    continue;
                case SSL_ERROR_SYSCALL:
                    if (errno == EINTR)
                        continue;
                    return -1;
                default:
                    return -1;
            }
        }
    }

}
