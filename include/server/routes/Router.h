#pragma once
#include <optional>
#include <server/routes/RouteTree.h>

namespace maziogra_http {
class Router {
private:
  RouteTree routes;
  bool exists(std::string path);
  std::optional<std::vector<std::string>> splitPath(std::string path);
public:
  Router() : routes() {};
  void insertRoute(std::string path, Route handler);
  RouteTree &getRoutes() { return routes; }
};
} // namespace maziogra_http
