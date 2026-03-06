#pragma once
#include <memory>
#include <server/types/RouteTypes.h>
#include <string>
#include <unordered_map>

namespace maziogra_http {

class RouteTree {
private:
  std::unordered_map<std::string, std::unique_ptr<RouteTree>> paths;

  std::string paramName;
  std::unique_ptr<RouteTree> param;

  Route handler;

public:
  RouteTree() : paramName("") {}

  Route &getHandler() { return handler; }
  void setHandler(Route h) { handler = std::move(h); }

  RouteTree *getParamChild() { return param.get(); }

  RouteTree *getStaticChild(const std::string &seg) {
    auto it = paths.find(seg);
    if (it != paths.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  void addStaticChild(const std::string &seg,
                      std::unique_ptr<RouteTree> child) {
    paths[seg] = std::move(child);
  }

  void setParamChild(std::string name, std::unique_ptr<RouteTree> child) {
    paramName = std::move(name);
    param = std::move(child);
  }

  const std::string &getParamName() const { return paramName; }
};

} // namespace maziogra_http
