#include <HttpResponse.h>

maziogra_http::HttpResponse::HttpResponse(const int statusCode, const std::string &message, const std::string &version)
    : statusCode(statusCode), version(version), message(message) {}

maziogra_http::HttpResponse::HttpResponse()
    : statusCode(200), version("HTTP/1.1"), message("OK") {}