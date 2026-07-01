#pragma once

#include <memory>
#include <vector>
#include <string>

#include <socket/Socket.h>
#include <socket/BaseSocket.h>
#include <socket/SecureSocket.h>

#include <server/routes/Router.h>
#include <server/types/RouteTypes.h>

namespace maziogra_http {

    class ServerHTTP {
    protected:
        static std::string default404Message;

        std::unique_ptr<Socket> s;
        Router router;
        std::vector<Middleware> middlewares;

        bool secure;

    public:
        explicit ServerHTTP(bool secure)
            : secure(secure)
        {
            if (secure) {
                s = std::make_unique<SecureSocket>();
            }
            else {
                s = std::make_unique<BaseSocket>();
            }
        }

        ~ServerHTTP() {
            if (s) {
                s->close();
            }
        }

        ServerHTTP(const ServerHTTP&) = delete;
        ServerHTTP& operator=(const ServerHTTP&) = delete;

        void start(int port);

        void start(int port,
            const char* certFile,
            const char* keyFile);

        void addRoute(std::string method,
            std::string path,
            Route route);

        void addMiddleware(Middleware middleware);

        static void setDefault404Message(const std::string& message) {
            default404Message = message;
        }

        static const std::string& getDefault404Message() {
            return default404Message;
        }

        Router& getRouter() { return router; }
    };

} // namespace maziogra_http