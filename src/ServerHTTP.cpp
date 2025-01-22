#include <HttpRequest.h>
#include <HttpResponse.h>
#include <ServerHTTP.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <unistd.h>

maziogra_http::ServerHTTP::ServerHTTP(const int port)
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

void maziogra_http::ServerHTTP::handleConnection(const int clientSock) {
  char buffer[1024] = {0};

  int received = recv(clientSock, buffer, sizeof(buffer), 0);
  if (received < 0) {
    std::cerr << "Error receiving data" << std::endl;
    close(clientSock);
    return;
  }

  HttpResponse res;
  HttpRequest req(std::string(buffer, sizeof buffer));
  for(auto& middleware : middlewares) {
    middleware(req, res); 
  }

  std::string search = req.getPath() + ":" + req.getMethod();
  std::cout << search << std::endl;
  auto route = routes.find(search);

  if (route != routes.end()) {
    res = route->second(req);
  } else {
    res = HttpResponse(404, "Not Found");
  }
  std::string temp = res.toString();
  send(clientSock, temp.c_str(), temp.size(), 0);
  close(clientSock);
}

void maziogra_http::ServerHTTP::addRoute(const std::string &path,
                                         const std::string &method,
                                         const HandlerFunction &handler) {
  std::string first = path + ":" + method;
  routes[first] = handler;
}

void maziogra_http::ServerHTTP::use(const Middleware &middleware) {
  middlewares.push_back(middleware);
}
