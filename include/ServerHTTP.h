#pragma once
#include <HttpRequest.h>
#include <HttpResponse.h>
#include <CoreSocket.h>
#include <functional>
#include <string>


namespace maziogra_http {
class ServerHTTP {
private:
  CoreSocket csock;
  std::map<std::string, std::function<HttpResponse(const HttpRequest&)>> routes;
public:
  ServerHTTP(const int port);

  void start();
  void handleConnection(const int clientSock);
  void addRoute(const std::string& path, const std::string& method, const std::function<HttpResponse(const HttpRequest&)>& handler);
};
} // namespace maziogra_http
