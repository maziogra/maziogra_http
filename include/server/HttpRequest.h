#pragma once
#include <map>
#include <stdexcept>
#include <string>

namespace maziogra_http {

    class HttpRequest {
    private:
        std::string method;
        std::string version;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;

    public:
        HttpRequest() = default;

        const std::string& getMethod() const { return method; }
        const std::string& getVersion() const { return version; }
        const std::string& getPath() const { return path; }
        const std::string& getBody() const { return body; }

        const std::map<std::string, std::string>& getHeaders() const {
            return headers;
        }

        void setMethod(const std::string& method) { this->method = method; }
        void setVersion(const std::string& version) { this->version = version; }
        void setPath(const std::string& path) { this->path = path; }
        void setBody(const std::string& body) { this->body = body; }

        void addHeader(const std::string& key, const std::string& value) {
            headers[key] = value;
        }

        void removeHeader(const std::string& key) {
            headers.erase(key);
        }
    };
} // namespace maziogra_http
