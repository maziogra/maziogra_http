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

    std::unique_ptr<HttpRequest> ServerThread::readRequest() {
        char tmp[4096];

        size_t headerEnd;

        while ((headerEnd = buffer.find("\r\n\r\n")) == std::string::npos) {

            ssize_t n = sock->receive(tmp, sizeof(tmp));
            if (n <= 0)
                return nullptr;

            buffer.append(tmp, n);
        }

        size_t contentLength = 0;

        // parse SOLO header
        HttpRequest headerOnly(buffer.substr(0, headerEnd + 4));

        auto headers = headerOnly.getHeaders();
        auto it = headers.find("Content-Length");

        if (it != headers.end()) {
            try {
                contentLength = std::stoul(it->second);
            } catch (...) {
                return nullptr; // header malformato
            }
        }

        size_t totalSize = headerEnd + 4 + contentLength;

        while (buffer.size() < totalSize) {

            ssize_t n = sock->receive(tmp, sizeof(tmp));
            if (n <= 0)
                return nullptr;

            buffer.append(tmp, n);
        }

        auto request = std::make_unique<HttpRequest>(
            buffer.substr(0, totalSize)
        );

        buffer.erase(0, totalSize);

        return request;
    }

    void ServerThread::run() {
        char buffer[1024];
        int tv = 30000;
        ::setsockopt(sock->getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        while (true) {
            auto requestPtr = readRequest();
            if (!requestPtr) {
                sock->close();
                return;
            }

            HttpRequest& request = *requestPtr;

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
            ssize_t bytes = sock->send(response.getRawResponse().c_str(), response.getRawResponse().size());
            if (bytes < 0) {
                sock->close();
                return;
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
}
