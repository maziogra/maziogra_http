#pragma once

#include <server/types/RouteTypes.h>
#include <socket/Socket.h>
#include <vector>
#include <thread>

namespace maziogra_http {

    class Router;

    class ServerThread {
    private:
        std::unique_ptr<Socket> sock;
        Router& router;
        std::vector<Middleware> middlewares;
        std::string buffer;
        Route r;

        void run();
        void executePipeline(int index, HttpRequest& req, HttpResponse& res);
        std::unique_ptr<HttpRequest> readRequest();

    public:
        ServerThread(
            std::unique_ptr<Socket> sock,
            Router& router,
            const std::vector<Middleware>& middlewares
        )
            : sock(std::move(sock)),
            router(router),
            middlewares(middlewares)
        {
            std::thread([this]() { this->run(); }).detach();
        }

        ServerThread(const ServerThread&) = delete;
        ServerThread& operator=(const ServerThread&) = delete;
    };

} // namespace maziogra_http