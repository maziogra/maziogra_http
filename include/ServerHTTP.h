#include <CoreSocket.h>
#include <functional>
#include <string>
#include <thread>
#include <sstream>
#include <string>

namespace maziogra_http {
class ServerHTTP {
private:
  CoreSocket csock;

public:
  ServerHTTP(int port);

  void start();
  void handleConnection(int);
  void addRoute(const std::string&, std::function<std::string()>);
};
} // namespace maziogra_http
