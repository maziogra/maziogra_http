#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace maziogra_http {
class CoreSocket {
private:
  int sock;
  int domain, type, protocol;
  int port;
  sockaddr_in addr;

public:
  CoreSocket(int domain, int type, int protocol, int port);
  ~CoreSocket();

  int getSock() const { return sock; }
  int getDomain() const { return domain; }
  int getType() const { return type; }
  int getProtocol() const { return protocol; }
  int getPort() const { return port; }
};
} // namespace maziogra_http
