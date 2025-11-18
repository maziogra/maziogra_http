#include <iostream>
#include <socket/BaseSocket.h>
#include <server/HttpRequest.h>
#include <server/ServerHTTP.h>

int main() {
    // maziogra_http::ServerHTTP server(0);
    // server.start(1984);
    maziogra_http::HttpRequest req("GET / HTTP/1.1");
    return 0;
}
