#include <ServerHTTP.h>
#include <sys/socket.h>

maziogra_http::ServerHTTP::ServerHTTP(int port)
    : csock(PF_INET, SOCK_STREAM, 0, port) {}

void maziogra_http::ServerHTTP::start() {
  int err = listen(csock.getSock(), 20);
  if (err < 0) {
    throw std::runtime_error("Error while listening");
  }
  std::cout << "Server listening on " << csock.getPort();

  sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);

  while (true) {
    int n_sock = accept(csock.getSock(), (sockaddr *)&clientAddr, &clientLen);
    if (n_sock < 0) {
      std::cerr << "Failed to accept client connection" << std::endl;
      continue;
    }
    std::thread(&ServerHTTP::handleConnection, this, n_sock).detach();
  }
}

void maziogra_http::ServerHTTP::handleConnection(int clientSock) {
  char buffer[1024] = {0};
  recv(clientSock, buffer, sizeof(buffer), 0);
  std::string httpResponse = "HTTP/1.1 200 OK\r\n"
                             "Content-Type: text/plain\r\n"
                             "Content-Length: 13\r\n"
                             "\r\n"
                             "Hello, World!";
  send(clientSock, httpResponse.c_str(), httpResponse.size(), 0);
  close(clientSock);
}
