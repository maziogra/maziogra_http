#pragma once
#include <stdexcept>
#include <server/HttpRequest.h>
#include <string>

namespace maziogra_http {
   
    class HttpRequestParser {
    private:
		void verifyPath(const HttpRequest& req);
		void verifyMethod(const HttpRequest& req);
		void verifyVersion(const HttpRequest& req);
    public:
        HttpRequestParser();

        void parse(const std::string& rawRequest, HttpRequest& req);
    };
} // namespace maziogra_http
