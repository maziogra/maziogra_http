#include <HttpRequest.h>
#include <ServerHTTP.h>
#include <sys/socket.h>

maziogra_http::ServerHTTP::ServerHTTP(int port)
    : csock(PF_INET, SOCK_STREAM, 0, port) {}

void maziogra_http::ServerHTTP::start() {
  int err = listen(csock.getSock(), 20);
  if (err < 0) {
    throw std::runtime_error("Error while listening");
  }
  std::cout << "Server listening on " << csock.getPort() << std::endl;

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

  int received = recv(clientSock, buffer, sizeof(buffer), 0);
  if (received < 0) {
    std::cerr << "Error receiving data" << std::endl;
    close(clientSock);
    return;
  }

  HttpRequest req(std::string(buffer, sizeof buffer));
  std::string search = req.getPath() + ":" + req.getMethod();
  std::cout << search << std::endl;
  auto route = routes.find(search);
  std::string res;
  if (route != routes.end()) {
    std::string temp = route->second(req);
    res = "HTTP/1.1 200\r\nContent Lenght " + std::to_string(temp.length()) +
          "\r\n\r\n" + temp;
  } else {
    res = "HTTP/1.1 404 Not Found\r\nContent Length: 5\r\n\r\nError";
  }
  send(clientSock, res.c_str(), res.size(), 0);
  close(clientSock);
}

void maziogra_http::ServerHTTP::addRoute(
    const std::string path, const std::string method,
    const std::function<std::string(const HttpRequest &)> handler) {
  std::string first = path + ":" + method;
  routes[first] = handler;
}
