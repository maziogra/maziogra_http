#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#endif

#include <iostream>
#include <stdexcept>

namespace maziogra_http {

  class CoreSocket {
  public:
    CoreSocket(int domain, int type, int protocol, int port);
    ~CoreSocket();

  private:
    int sock;
    int domain, type, protocol, port;
    struct sockaddr_in addr;
  };

  CoreSocket::CoreSocket(int domain, int type, int protocol, int port)
      : domain(domain), type(type), protocol(protocol), port(port) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      throw std::runtime_error("WSAStartup failed");
    }
#endif

    sock = socket(domain, type, protocol);
    if (sock < 0)
      throw std::runtime_error("Error while creating socket");

    std::cout << "Socket created" << std::endl;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(addr.sin_zero, 0, sizeof addr.sin_zero);

    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
      throw std::runtime_error("Error while binding");

    std::cout << "Socket binded" << std::endl;
  }

  CoreSocket::~CoreSocket() {
    if (sock != 0) {
#ifdef _WIN32
      closesocket(sock);
      WSACleanup();
#else
      close(sock);
#endif
    }
  }

} // namespace maziogra_http