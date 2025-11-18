#ifndef SERVERHTTP_H
#define SERVERHTTP_H
#include <functional>
#include <socket/Socket.h>
#include <socket/BaseSocket.h>


namespace maziogra_http {
    class ServerHTTP {
    protected:
        std::unique_ptr<Socket> s;
        int port;
        // TODO map of routes and list of middlewares
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
        // TODO fix this methods, these are not completed
        // void addRoute();
        // void removeRoute();
        // void addMiddleware();
        // void removeMiddleware();
    };
}


#endif //SERVERHTTP_H
