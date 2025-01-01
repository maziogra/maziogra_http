#include <HttpRequest.h>

maziogra_http::HttpRequest::HttpRequest(const std::string &s) {
  std::istringstream stream(s);
  stream >> method >> path >> version;

  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    int pos = line.find(":");
    if (pos != std::string::npos) {
      std::string name = line.substr(0, pos);
      std::string value = line.substr(pos + 1);
      headers[name] = value;
    }
  }

  if (stream) {
    std::getline(stream, body, '\0');
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
