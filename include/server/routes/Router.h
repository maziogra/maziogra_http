#pragma once
#include <optional>
#include <server/routes/RouteTree.h>

namespace maziogra_http {

struct MatchResult {
    bool found;
    std::unordered_map<std::string, std::string> params;
};

class Router {
private:
  RouteTree routes;
  bool canInsert(const std::string &path);
  MatchResult match(const std::string& path);
  std::optional<std::vector<std::string>> splitPath(const std::string &path);
  RouteTree& getRoutes() { return routes; }
public:
  Router() : routes() {};
  void insertRoute(std::string path, Route handler);
};
} // namespace maziogra_http
