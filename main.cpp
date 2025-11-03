#include <iostream>
#include "include/socket/BaseSocket.h"
#include "server/ServerHTTP.h"

int main() {
    maziogra_http::ServerHTTP server(0);
    server.start(1984);
    return 0;
}
