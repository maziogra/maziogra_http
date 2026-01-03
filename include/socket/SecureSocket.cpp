#include <socket/Socket.h>

namespace maziogra_http {
    class SecureSocket : public Socket {
        public:
            SecureSocket();
            ~SecureSocket() = default;
            
            bool create(int port) override;
            void close() override;
            std::unique_ptr<Socket> accept() override;
            int send(const char* data, size_t size) override;
            int receive(char* buffer, size_t size) override;


            SecureSocket(const SecureSocket&) = delete;
            SecureSocket& operator=(const SecureSocket&) = delete;
            SecureSocket(SecureSocket&) noexcept = default;
            SecureSocket& operator=(SecureSocket&&) noexcept = default;
    };
}
