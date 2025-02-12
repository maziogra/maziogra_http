#include <HttpRequest.h>
#include <iostream>
#include <sstream>

maziogra_http::HttpRequest::HttpRequest(const std::string &s) {
  std::istringstream stream(s);
  stream >> method >> path >> version;
  if (stream) {
    std::getline(stream, body, '\0');
  }
  parseHeaders(stream);
  parseUrlParams();
}

maziogra_http::HttpRequest::HttpRequest(
    const std::string &method, const std::string &path,
    const std::string &version,
    const std::map<std::string, std::string> &headers,
    const std::map<std::string, std::string> &urlParams,
    const std::string &body)
    : method(method), path(path), version(version), headers(headers),
      urlParams(urlParams), body(body) {}

void maziogra_http::HttpRequest::parseHeaders(std::istringstream &stream) {
  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    int pos = line.find(":");
    if (pos != std::string::npos) {
      std::string name = line.substr(0, pos);
      std::string value = line.substr(pos + 1);
      headers[name] = value;
    }
  }
}

void maziogra_http::HttpRequest::parseUrlParams() {
  int pos = path.find("?");

  if (pos == std::string::npos) {
    return;
  }

  std::string query = path.substr(pos + 1);
  path = path.substr(0, pos);

  std::istringstream ss(query);
  std::string temp;
  while (std::getline(ss, temp, '&')) {
    int equal = temp.find("=");

    if (equal != std::string::npos) {
      std::string key = temp.substr(0, equal);
      std::string value = temp.substr(equal + 1);
      urlParams[key] = value;
    }
  }
}

void maziogra_http::HttpRequest::printRequest() const {
  std::cout << "Method: " << method << "\n";
  std::cout << "Path: " << path << "\n";
  std::cout << "Version: " << version << "\n";
  std::cout << "Headers:\n";
  for (const auto &header : headers) {
    std::cout << "  " << header.first << ": " << header.second << "\n";
  }
  std::cout << "Body: " << body << "\n";
}
