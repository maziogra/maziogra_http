#pragma once
#include <string>

namespace maziogra_http {
class HttpResponse {
private:
  int statusCode;
  std::string version;
  std::string message;

public:
  HttpResponse();
  HttpResponse(const int statusCode, const std::string &version,
               const std::string &message);
  HttpResponse(const int statusCode, const std::string &message);
  const int getStatusCode() const { return statusCode; }
  const std::string &getMessage() const { return message; }
  const std::string &getVersion() const { return version; }

  void setStatusCode(const int statusCode) { this->statusCode = statusCode; }
  void setMessage(const std::string &message) { this->message = message; }
  void setVersion(const std::string &version) { this->version = version; }

  HttpResponse &operator=(const HttpResponse &other) {
    if(this != &other) {
      statusCode = other.statusCode;
      version = other.version;
      message = other.message;
    }
    return *this;
  }

  std::string toString() const {
    return version + " " + std::to_string(statusCode) + "\r\n\r\n" + message;
  }
};
} // namespace maziogra_http
