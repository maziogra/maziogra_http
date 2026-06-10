#include <optional>
#include <server/routes/Router.h>

namespace maziogra_http {
// void Router::insertRoute(std::string path, Route handler) {}

bool Router::canInsert(const std::string &path) {
	auto segments = this->splitPath(path);
	if (segments.has_value()) {
		RouteTree* current = &routes;
		for (const auto& seg : segments.value()) {
			if (seg.at(0) == ':') {
				current = current->getParamChild();
			}
			else {
				current = current->getStaticChild(seg);
			}
			if (!current) return false;
		}
		return true;
	}
}

MatchResult Router::match(const std::string& path) {
    MatchResult result;
    result.found = false;

    auto segments = splitPath(path);
    if (!segments) return result;

    RouteTree* current = &routes;

    for (const auto& seg : *segments) {

        RouteTree* next = current->getStaticChild(seg);

        if (next) {
            current = next;
            continue;
        }

        next = current->getParamChild();
        if (!next) return result;

        std::string paramName = next->getParamName();
        result.params[paramName] = seg;

        current = next;
    }

    result.found = true;
    return result;
}

std::optional<std::vector<std::string>> Router::splitPath(const std::string &path) {
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
