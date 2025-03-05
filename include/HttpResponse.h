#pragma once
#include <string>
#include <map>

namespace maziogra_http {
  class HttpResponse {
  private:
    int statusCode;
    std::string version;
    std::string message;
    std::map<std::string, std::string> headers;

  public:
    HttpResponse();
    HttpResponse(const int statusCode, const std::string &message, const std::string &version = "HTTP/1.1");
    const int getStatusCode() const { return statusCode; }
    const std::string &getMessage() const { return message; }
    const std::string &getVersion() const { return version; }

    void setStatusCode(const int statusCode) { this->statusCode = statusCode; }
    void setMessage(const std::string &message) { this->message = message; }
    void setVersion(const std::string &version) { this->version = version; }

    void addHeader(const std::string &key, const std::string &value) {
      headers[key] = value;
    }

    std::string toString() const {
      std::string response = version + " " + std::to_string(statusCode) + " " + message + "\r\n";

      for (const auto& header : headers) {
        response += header.first + ": " + header.second + "\r\n";
      }

      response += "\r\n";
      return response + message;
    }
  };
} // namespace maziogra_http
