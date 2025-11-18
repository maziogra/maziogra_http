#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <map>
#include <string>
#include <vector>


namespace maziogra_http {
    class HttpRequest {
    protected:
        std::string method;
        std::string version;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;
        void parseRequest(std::string rawRequest);
    public:
        HttpRequest();
        HttpRequest(std::string rawRequest) {
            parseRequest(std::move(rawRequest));
        }

        std::string getMethod() const {
            return method;
        }

        void setMethod(std::string method) {
            this->method = std::move(method);
        }

        std::string getVersion() const {
            return version;
        }

        void setVersion(std::string version) {
            this->version = std::move(version);
        }

        std::string getPath() const {
            return path;
        }

        void setPath(std::string path) {
            this->path = std::move(path);
        }

        std::map<std::string, std::string> getHeaders() const {
            return headers;
        }

        void setHeaders(std::map<std::string, std::string> headers) {
            this->headers = std::move(headers);
        }

        std::string getBody() const {
            return body;
        }

        void setBody(std::string body) {
            this->body = std::move(body);
        }
    };
}

#endif