#pragma once
#include <map>
#include <string>

namespace maziogra_http {
class HttpRequest {
private:
  std::string method;
  std::string path;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;

public:
  HttpRequest(const std::string &);
  HttpRequest(const std::string &method, const std::string &path,
              const std::string &version,
              const std::map<std::string, std::string> &headers,
              const std::string &body);
  void printRequest() const;

  const std::string &getMethod() const { return method; }
  const std::string &getPath() const { return path; }
  const std::string &getVersion() const { return version; }
  const std::map<std::string, std::string> &getHeaders() const {
    return headers;
  }
  const std::string &getBody() const { return body; }

  void setMethod(std::string &method) { this->method = method; }
  void setPath(std::string &path) { this->path = path; }
  void setVersion(std::string &version) { this->version = version; }
  void setHeaders(std::map<std::string, std::string>& headers) {
    this->headers = headers;
  }
  void setBody(const std::string &body) { this->body = body; }
};
} // namespace maziogra_http
