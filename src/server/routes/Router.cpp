#include <optional>
#include <server/routes/Router.h>

namespace maziogra_http {
	void Router::insertRoute(std::string method, std::string path, Route handler) {
		if (path.empty() || path[0] != '/')
			throw std::invalid_argument("Path must start with '/'");

		auto segments = splitPath(path);
		if (!segments) return;

		RouteTree* current = &routes;

		for (const auto& seg : *segments) {

			if (!seg.empty() && seg[0] == ':') {

				auto* next = current->getParamChild();

				if (next) {
					if (current->getParamName() != seg.substr(1)) {
						throw std::runtime_error("Conflicting param route at same level");
					}
				}
				else {
					auto node = std::make_unique<RouteTree>();
					next = node.get();
					current->setParamChild(seg.substr(1), std::move(node));
				}

				current = next;
			}
			else {
				auto* next = current->getStaticChild(seg);

				if (!next) {
					auto node = std::make_unique<RouteTree>();
					next = node.get();
					current->addStaticChild(seg, std::move(node));
				}

				current = next;
			}
		}

		current->setHandler(method, std::move(handler));
	}

	bool Router::canInsert(const std::string& path) {
		auto segments = splitPath(path);
		if (!segments) return false;

		RouteTree* current = &routes;

		for (const auto& seg : *segments) {

			if (!seg.empty() && seg[0] == ':') {

				if (current->getParamChild()) {
					if (current->getParamName() != seg.substr(1))
						return false;
				}

				current = current->getParamChild();
			}
			else {
				current = current->getStaticChild(seg);
			}

			if (!current)
				return false;
		}

		return true;
	}

	MatchResult Router::match(const std::string& method, const std::string& path) {
		MatchResult result;

		auto segments = splitPath(path);
		if (!segments) return result;

		RouteTree* current = &routes;

		for (const auto& seg : *segments) {

			if (auto* next = current->getStaticChild(seg)) {
				current = next;
				continue;
			}

			auto* paramNode = current->getParamChild();
			if (!paramNode)
				return result;

			result.params[paramNode->getParamName()] = seg;
			current = paramNode;
		}

		if (auto* h = current->getHandler(method)) {
			result.handler = h;
			result.found = true;
		}

		return result;
	}

	std::optional<std::vector<std::string>> Router::splitPath(const std::string& path) {
		if (path.empty() || path[0] != '/')
			return std::nullopt;

		std::vector<std::string> segments;

		size_t start = 1;

		while (true) {
			size_t pos = path.find('/', start);

			if (pos == std::string::npos) {
				if (start < path.size())
					segments.push_back(path.substr(start));
				break;
			}

			if (pos > start)
				segments.push_back(path.substr(start, pos - start));

			start = pos + 1;
		}

		return segments;
	}

} // namespace maziogra_http
