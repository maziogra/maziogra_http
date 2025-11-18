#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <map>
#include <string>

namespace maziogra_http {
    class HttpResponse {
    protected:
        std::string version = "HTTP/1.1";
        std::map<std::string, std::string> headers;
        int statusCode;
        std::string body;
    public:
        HttpResponse(int statusCode) : statusCode(statusCode) {};
        HttpResponse(int statusCode, std::string body) : statusCode(statusCode), body(body) {
            headers["Content-Type"] = "application/json";
            headers["Content-Length"] = std::to_string(body.length());
        };
        HttpResponse();

        void addHeader(const std::string &key, const std::string &value) {
            headers[key] = value;
        }

        void removeHeader(const std::string &key) {
            auto it = headers.find(key);
            if (it != headers.end()) {
                headers.erase(it);
            }
        }

        std::string getVersion() const {
            return version;
        }

        void setVersion(const std::string &version) {
            this->version = version;
        }

        std::map<std::string, std::string> getHeaders() const {
            return headers;
        }

        void setHeaders(const std::map<std::string, std::string> &headers) {
            this->headers = headers;
        }

        int getStatusCode() const {
            return statusCode;
        }

        void setStatusCode(int status_code) {
            statusCode = status_code;
        }

        std::string getBody() const {
            return body;
        }

        void setBody(const std::string &body) {
            this->body = body;
        }

        HttpResponse & operator=(const HttpResponse &other) {
            if (this == &other)
                return *this;
            version = other.version;
            headers = other.headers;
            statusCode = other.statusCode;
            body = other.body;
            return *this;
        }

        HttpResponse & operator=(HttpResponse &&other) noexcept {
            if (this == &other)
                return *this;
            version = std::move(other.version);
            headers = std::move(other.headers);
            statusCode = other.statusCode;
            body = std::move(other.body);
            return *this;
        }
    };
}

#endif //HTTPRESPONSE_H
