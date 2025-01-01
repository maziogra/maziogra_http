#include <HttpRequest.h>
#include <CoreSocket.h>
#include <functional>
#include <string>
#include <thread>
#include <sstream>

namespace maziogra_http {
class ServerHTTP {
private:
  CoreSocket csock;
  std::map<std::string, std::function<std::string(HttpRequest&)>> routes;
public:
  ServerHTTP(int port);

  void start();
  void handleConnection(int);
  void addRoute(const std::string, const std::string, const std::function<std::string(const HttpRequest&)>);
};
} // namespace maziogra_http
