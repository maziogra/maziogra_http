#include <server/HttpRequestParser.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

namespace maziogra_http {
    HttpRequestParser::HttpRequestParser() = default;

    void HttpRequestParser::verifyPath(const HttpRequest& req) {
        const std::string& path = req.getPath();

        if (path.empty() || path[0] != '/')
            throw std::invalid_argument("Invalid path, it's empty or doesn't start with '/'");

        for (char c : path) {
            if (c <= 32 || c == 127)
                throw std::invalid_argument("Invalid path: contains control chars");
        }
    }

    void HttpRequestParser::verifyMethod(const HttpRequest& req) {
        static const std::unordered_set<std::string> valid = {
            "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH"
        };

        if (valid.find(req.getMethod()) == valid.end())
            throw std::invalid_argument("Invalid HTTP method");
    }

    void HttpRequestParser::verifyVersion(const HttpRequest& req) {
        if (req.getVersion() != "HTTP/1.0" && req.getVersion() != "HTTP/1.1") {
            throw std::invalid_argument("Invalid HTTP version");
        }
    }

    void HttpRequestParser::parse(const std::string& rawRequest, HttpRequest& req) {
        std::stringstream ss(rawRequest);
        std::string l;

        if (std::getline(ss, l) && !l.empty()) {
            std::stringstream line(l);
			std::string method, path, version;
            if (!(line >> method >> path >> version)) {
                throw std::invalid_argument("Invalid request line");
            }
            req.setMethod(method);
            req.setPath(path);
            req.setVersion(version);
        }
        else {
            throw std::invalid_argument("Empty request");
        }
        
        verifyPath(req);
        verifyVersion(req);
        verifyMethod(req);


        while (std::getline(ss, l) && l != "\r" && !l.empty()) {
            if (l.back() == '\r') l.pop_back();

            int pos = l.find(':');
            if (pos == std::string::npos) continue;

            std::string key = l.substr(0, pos);
            std::string value = l.substr(pos + 1);

            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value.erase(0, 1);

            while (!value.empty() && (value.back() == ' ' || value.back() == '\t'))
                value.pop_back();

            req.addHeader(key, value);
        }

        std::string bodyBuffer;
        std::getline(ss, bodyBuffer, '\0');

        if (bodyBuffer.size() >= 2 && bodyBuffer[0] == '\r' && bodyBuffer[1] == '\n')
            bodyBuffer.erase(0, 2);

        req.setBody(std::move(bodyBuffer));
    }
}