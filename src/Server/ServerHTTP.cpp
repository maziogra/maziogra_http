#include <iostream>
#include <server/ServerHTTP.h>

#include "server/ServerThread.h"

namespace maziogra_http {
    void ServerHTTP::start(int port) {
        s->create(port);
        std::cout << "Server started on port " << port << std::endl;
        std::vector<std::shared_ptr<ServerThread>> clients;


        while (true) {
            auto client = s->accept();
            if (client == nullptr) {continue;}
            std::cout << "Client connected" << std::endl;
            auto serverThread = std::make_shared<ServerThread>(
                std::move(client),
                routes,
                middlewares
            );
            clients.push_back(serverThread);
        }
    }

    void ServerHTTP::addRoute(std::string method, std::string path, Route route) {
        routes[method + ":" + path] = route;
    }

    void ServerHTTP::addMiddleware(Middleware middleware) {
        middlewares.push_back(middleware);
    }

}
