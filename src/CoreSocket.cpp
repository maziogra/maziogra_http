#include <CoreSocket.h>

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

maziogra_http::CoreSocket::CoreSocket(const int domain, const int type,
                                      const int protocol, const int port)
    : domain(domain), type(type), protocol(protocol), port(port) {
  sock = socket(domain, type, protocol);
  if (sock < 0)
    throw std::runtime_error("Error while creating socket");

  std::cout << "Socket created" << std::endl;

  addr.sin_family = AF_UNSPEC;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(addr.sin_zero, 0, sizeof addr.sin_zero);

  int err = bind(sock, (sockaddr *)&addr, sizeof addr);
  if (err < 0)
    throw std::runtime_error("Error while binding");

  std::cout << "Socket binded" << std::endl;
}

maziogra_http::CoreSocket::~CoreSocket() {
  if (sock == 0)
    close(sock);
}
