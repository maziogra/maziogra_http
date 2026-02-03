#include <iostream>
#include <server/ServerHTTP.h>
#include <server/ServerThread.h>

namespace maziogra_http {
  std::string ServerHTTP::default404Message = "<h1>NOT FOUND</h1>";

  void ServerHTTP::start(int port) {
    s->create(port);
    std::cout << "Server started on port " << port << std::endl;
    std::vector<std::shared_ptr<ServerThread>> clients;

    while (true) {
      auto client = s->accept();
      if (client == nullptr) {
        continue;
      }
      std::cout << "Client connected" << std::endl;
      auto serverThread =
          std::make_shared<ServerThread>(std::move(client), routes, middlewares);
      clients.push_back(serverThread);
    }
  }

  void ServerHTTP::start(int port,
                         const char* certFile,
                         const char* keyFile) {

    if (!secure) {
      std::cerr << "Server not configured for TLS\n";
      return;
    }

    SecureSocket* secureSocket = (SecureSocket*)(s.get());

    if (!secureSocket->create(port, certFile, keyFile)) {
      std::cerr << "Failed to start HTTPS server\n";
      return;
    }

    std::cout << "HTTPS Server started on port " << port << std::endl;

    std::vector<std::shared_ptr<ServerThread>> clients;

    while (true) {
      auto client = s->accept();
      if (!client) continue;

      std::cout << "Secure client connected" << std::endl;

      auto serverThread = std::make_shared<ServerThread>(std::move(client), routes, middlewares);

      clients.push_back(serverThread);
    }
  }


  void ServerHTTP::addRoute(std::string method, std::string path, Route route) {
    routes[method + ":" + path] = route;
  }

  void ServerHTTP::addMiddleware(Middleware middleware) {
    middlewares.push_back(middleware);
  }

} // namespace maziogra_http
