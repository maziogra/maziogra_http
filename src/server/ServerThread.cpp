#include <server/HttpRequest.h>
#include <cstring>
#include <iostream>
#include <map>
#include <server/ServerHTTP.h>
#include <server/ServerThread.h>
#include <server/HttpRequestParser.h>
#if _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif


namespace maziogra_http {
void ServerThread::executePipeline(int index, HttpRequest &req,
                                   HttpResponse &res) {
  if (index < middlewares.size()) {
    Middleware &mw = middlewares[index];
    mw(req, res, [&]() { executePipeline(index + 1, req, res); });
  } else {
    r(req, res);
  }
}

std::unique_ptr<HttpRequest> ServerThread::readRequest() {
    HttpRequestParser parser;

    char tmp[4096];

    size_t headerEnd;

    while ((headerEnd = buffer.find("\r\n\r\n")) == std::string::npos) {

        std::ptrdiff_t n = sock->receive(tmp, sizeof(tmp));

        if (n <= 0)
            return nullptr;

        buffer.append(tmp, n);
    }

    HttpRequest headerOnly;

    parser.parse(buffer.substr(0, headerEnd + 4), headerOnly);

    size_t contentLength = 0;

    auto headers = headerOnly.getHeaders();

    auto it = headers.find("Content-Length");

    if (it != headers.end()) {
        try {
            contentLength = std::stoul(it->second);
        }
        catch (...) {
            return nullptr;
        }
    }


    size_t totalSize = headerEnd + 4 + contentLength;

    while (buffer.size() < totalSize) {

        std::ptrdiff_t n = sock->receive(tmp, sizeof(tmp));

        if (n <= 0)
            return nullptr;

        buffer.append(tmp, n);
    }


    auto request = std::make_unique<HttpRequest>();

    parser.parse(buffer.substr(0, totalSize), *request);

    buffer.erase(0, totalSize);

    return request;
}

void ServerThread::run() {
  char buffer[1024];
  int tv = 30000;
  ::setsockopt(sock->getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv,
               sizeof(tv));
  while (true) {
    try {
      auto requestPtr = readRequest();

      if (!requestPtr) {
        sock->close();
        return;
      }

      HttpRequest &request = *requestPtr;

      HttpResponse response;
      std::string key = request.getMethod() + ":" + request.getPath();
      if (routes.contains(key)) {
        r = routes[key];
        executePipeline(0, request, response);
      } else {
        response.setStatusCode(404);
        response.setBody(ServerHTTP::getDefault404Message());
      }
      response.addHeader("Content-Length",
                         std::to_string(std::size(response.getBody())));
      std::ptrdiff_t bytes = sock->send(response.getRawResponse().c_str(),
                                 response.getRawResponse().size());

      auto& headers = request.getHeaders();
      auto it = headers.find("Connection");

      if ((it != headers.end() && it->second == "close") || (it == headers.end() && request.getVersion() == "HTTP/1.0")) {
          sock->close();
          return;
      }

      if (bytes < 0) {
        sock->close();
        return;
      }
      memset(buffer, 0, sizeof(buffer));
    } catch (...) {
      std::cerr << "Unknown error, closing connection\n";
      sock->close();
      return;
    }
  }
}
} // namespace maziogra_http
