#include <optional>
#include <server/routes/Router.h>

namespace maziogra_http {
// void Router::insertRoute(std::string path, Route handler) {}

// bool Router::exists(std::string path) {}

std::optional<std::vector<std::string>> splitPath(const std::string &path) {
  std::vector<std::string> segments;

  if (path.empty() || path[0] != '/')
    return std::nullopt;

  int start = 1;
  int pos;

  while ((pos = path.find('/', start)) != std::string::npos) {
    if (pos > start) {
      segments.emplace_back(path.substr(start, pos - start));
    }
    start = pos + 1;
  }

  if (start < path.size()) {
    segments.emplace_back(path.substr(start));
  }

  return segments;
}
} // namespace maziogra_http
