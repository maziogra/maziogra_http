#pragma once
#include <CoreSocket.h>
#include <HttpRequest.h>
#include <HttpResponse.h>
#include <functional>
#include <string>
#include <vector>

namespace maziogra_http {
class ServerHTTP {
private:
  using Middleware = std::function<void(HttpRequest &, HttpResponse &)>;
  using HandlerFunction = std::function<HttpResponse(const HttpRequest &)>;
  CoreSocket csock;
  std::map<std::string, HandlerFunction> routes;
  std::vector<Middleware> middlewares;

public:
  ServerHTTP(const int port);

  void start();
  void handleConnection(const int clientSock);
  void use(const Middleware &middleware);
  void addRoute(const std::string &path, const std::string &method,
                const HandlerFunction &handler);
};
} // namespace maziogra_http
