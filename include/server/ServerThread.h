#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include <map>
#include <thread>
#include <socket/Socket.h>

#include <server/ServerHTTP.h>

namespace maziogra_http {
    class ServerThread {
    private:
        std::unique_ptr<Socket> sock;
        std::map<std::string, Route> routes;
        std::vector<Middleware> middlewares;
        Route r;

        void run();
        void executePipeline(int index, HttpRequest& req, HttpResponse& res);

    public:
        ServerThread(
            std::unique_ptr<Socket> sock,
            const std::map<std::string, Route>& routes,
            const std::vector<Middleware> &middlewares
            )
        : sock(std::move(sock)), routes(routes), middlewares(middlewares) {
            std::thread([this]() { this->run(); }).detach();
        }

        ServerThread(const ServerThread&) = delete;
        ServerThread& operator=(const ServerThread&) = delete;
    };
}


#endif