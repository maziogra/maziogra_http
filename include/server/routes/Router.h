#pragma once
#include <optional>
#include <server/routes/RouteTree.h>

namespace maziogra_http {

    struct MatchResult {
        bool found = false;
        std::unordered_map<std::string, std::string> params;
        Route* handler = nullptr;
    };

    class Router {
    private:
        RouteTree routes;

        bool canInsert(const std::string& path);
        std::optional<std::vector<std::string>> splitPath(const std::string& path);

    public:
        Router() = default;

        void insertRoute(std::string method, std::string path, Route handler);
        MatchResult match(const std::string& method, const std::string& path);
    };

}