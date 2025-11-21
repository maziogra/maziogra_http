#include <iostream>
#include <server/ServerHTTP.h>

namespace maziogra_http {
    void ServerHTTP::start(int port) {
        s->create(port);
        while (true) {
            auto client = s->accept();
            if (client == nullptr) {continue;}
            char* buffer = new char[1024];
            client->receive(buffer, 1024);
            std::cout << buffer << std::endl;
            client->close();
        }
    }

    void ServerHTTP::addRoute(std::string method, std::string path, Route route) {
        routes[method + ":" + path] = route;
    }

    void ServerHTTP::addMiddleware(Middleware middleware) {
        middlewares.push_back(middleware);
    }

}