#ifndef SERVERHTTP_H
#define SERVERHTTP_H
#include <functional>
#include <socket/Socket.h>
#include <socket/BaseSocket.h>
#include <map>
#include <vector>
#include <server/HttpRequest.h>
#include <server/HttpResponse.h>

namespace maziogra_http {
    using Route = std::function<void(HttpRequest& request, HttpResponse& response)>;
    using Middleware = std::function<void(HttpRequest& request, HttpResponse& response, std::function<void()> callback)>;
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
                // s = std::make_unique<SecureSocket>();
            } else
                s = std::make_unique<BaseSocket>();
        }
        ~ServerHTTP() {
            s->close();
        };

        void start(int port);
        void addRoute(std::string method, std::string path, Route lambda);
        void addMiddleware(Middleware middleware);
        static void setDefault404Message(std::string message) {default404Message = message;}
        static std::string getDefault404Message() {return default404Message;}
    };
}


#endif //SERVERHTTP_H
