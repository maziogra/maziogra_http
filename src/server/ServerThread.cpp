#include <cstring>
#include <iostream>
#include <server/ServerThread.h>
#include <sys/socket.h>
#include <map>

namespace maziogra_http {
    void ServerThread::executePipeline(int index, HttpRequest& req, HttpResponse& res) {
        if (index < middlewares.size()) {
            Middleware& mw = middlewares[index];
            mw(req, res, [&] () {
                executePipeline(index + 1, req, res);
            });
        } else {
            r(req, res);
        }
    }

    void ServerThread::run() {
        char buffer[1024];
        int tv = 30000;
        ::setsockopt(sock->getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        while (true) {
            int bytes = sock->receive(buffer, std::size(buffer));
            // check if the error is because of timeout
            if (bytes < 0) {
                // if (errno == EAGAIN || errno == EWOULDBLOCK)
                sock->close();
                return;
            }

            HttpRequest request(buffer);
            if (request.getHeaders().contains("Connection")) {
                if (request.getHeaders()["Connection"] == "close") {
                    sock->close();
                    return;
                }
            }

            HttpResponse response;
            std::string key = request.getMethod() + ":" + request.getPath();
            if (routes.contains(key)) {
                r = routes[key];
                executePipeline(0, request, response);
            } else {
                response.setStatusCode(404);
                response.setBody(ServerHTTP::getDefault404Message());
            }
            response.addHeader("Content-Length", std::to_string(std::size(response.getBody())));
            sock->send(response.getRawResponse().c_str(), response.getRawResponse().size());
            memset(buffer, 0, sizeof(buffer));
        }
    }
}
