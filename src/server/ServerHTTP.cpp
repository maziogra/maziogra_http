#include <iostream>
#include <memory>
#include <vector>

#include <server/ServerHTTP.h>
#include <server/ServerThread.h>

namespace maziogra_http {

    std::string ServerHTTP::default404Message = "<h1>NOT FOUND</h1>";

    void ServerHTTP::start(int port) {
        if (!s->create(port)) {
            std::cerr << "Failed to start HTTP server\n";
            return;
        }

        std::cout << "Server started on port " << port << '\n';

        std::vector<std::shared_ptr<ServerThread>> clients;

        while (true) {
            auto client = s->accept();

            if (!client)
                continue;

            std::cout << "Client connected\n";

            auto serverThread = std::make_shared<ServerThread>(
                std::move(client),
                router,
                middlewares
            );

            clients.push_back(serverThread);
        }
    }

    void ServerHTTP::start(int port,
        const char* certFile,
        const char* keyFile) {

        if (!secure) {
            std::cerr << "Server not configured for TLS\n";
            return;
        }

        auto* secureSocket = static_cast<SecureSocket*>(s.get());

        if (!secureSocket->create(port, certFile, keyFile)) {
            std::cerr << "Failed to start HTTPS server\n";
            return;
        }

        std::cout << "HTTPS Server started on port " << port << '\n';

        std::vector<std::shared_ptr<ServerThread>> clients;

        while (true) {
            auto client = s->accept();

            if (!client)
                continue;

            std::cout << "Secure client connected\n";

            auto serverThread = std::make_shared<ServerThread>(
                std::move(client),
                router,
                middlewares
            );

            clients.push_back(serverThread);
        }
    }

    void ServerHTTP::addRoute(std::string method,
        std::string path,
        Route route) {
        router.insertRoute(std::move(method),
            std::move(path),
            std::move(route));
    }

    void ServerHTTP::addMiddleware(Middleware middleware) {
        middlewares.push_back(std::move(middleware));
    }

} // namespace maziogra_http