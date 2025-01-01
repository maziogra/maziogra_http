#pragma once
#include <iostream>
#include <map>
#include <sstream>
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
  void printRequest() const;

  const std::string& getMethod() const { return method; }
  const std::string& getPath() const { return path; }
  const std::string& getVersion() const {return version; }
  const std::map<std::string, std::string>& getHeaders() const { return headers; }
};
} // namespace maziogra_http
