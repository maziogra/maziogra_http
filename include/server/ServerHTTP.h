#pragma once
#include <socket/Socket.h>
#include <socket/BaseSocket.h>
#include <server/types/RouteTypes.h>
#include <socket/SecureSocket.h>

namespace maziogra_http {
    class ServerHTTP {
    protected:
        static std::string default404Message;
        std::unique_ptr<Socket> s;
        int port;
        std::map<std::string, Route> routes;
        std::vector<Middleware> middlewares;
        // 0 no 1 yes
        int secure;
    public:
        ServerHTTP(int secure) : secure(secure) {
            if (secure) {
                s = std::make_unique<SecureSocket>();
            } else
                s = std::make_unique<BaseSocket>();
        }
        ~ServerHTTP() {
            s->close();
        };

        void start(int port);
        void start(int port,
           const char* certFile,
           const char* keyFile);

        void addRoute(std::string method, std::string path, Route lambda);
        void addMiddleware(Middleware middleware);
        static void setDefault404Message(std::string message) {default404Message = message;}
        static std::string getDefault404Message() {return default404Message;}
    };
}
