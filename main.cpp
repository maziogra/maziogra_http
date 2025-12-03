#include <iostream>
#include <socket/BaseSocket.h>
#include <server/HttpRequest.h>
#include <server/ServerHTTP.h>

int main() {
    maziogra_http::ServerHTTP server(0);
    server.addRoute("GET", "/home", [](maziogra_http::HttpRequest& req, maziogra_http::HttpResponse& res) {
        res.setStatusCode(200);
        res.setBody("<html><body><h1>Benvenuto su /home!</h1></body></html>");
    });
    server.start(1984);
}