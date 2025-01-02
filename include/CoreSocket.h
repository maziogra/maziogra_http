#pragma once

#include <netinet/in.h>

namespace maziogra_http {
class CoreSocket {
private:
  int sock;
  int domain, type, protocol;
  int port;
  sockaddr_in addr;

public:
  CoreSocket(const int domain, const int type, const int protocol,
             const int port);
  ~CoreSocket();

  int getSock() const { return sock; }
  int getDomain() const { return domain; }
  int getType() const { return type; }
  int getProtocol() const { return protocol; }
  int getPort() const { return port; }
};
} // namespace maziogra_http
