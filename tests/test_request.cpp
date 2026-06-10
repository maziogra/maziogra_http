#include <catch2/catch_test_macros.hpp>
#include <server/HttpRequest.h>
#include <server/HttpRequestParser.h>
#include <server/ServerHTTP.h>
#include <utils/NetInit.h>

#include <thread>
#include <chrono>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace maziogra_http;

static std::string sendRequest(const std::string& request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    REQUIRE(sock >= 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3968);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    REQUIRE(connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0);

    send(sock, request.c_str(), (int)request.size(), 0);

    char buffer[4096];
    int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    REQUIRE(n > 0);

    buffer[n] = '\0';

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return std::string(buffer);
}

TEST_CASE("BasicCompleteRequest") {
    NetInit netInit;

    maziogra_http::ServerHTTP server(0);

    server.addRoute("GET", "/home",
        [](maziogra_http::HttpRequest& req, maziogra_http::HttpResponse& res) {
            res.setStatusCode(200);
            res.setBody("<html><body><h1>Benvenuto su /home!</h1></body></html>");
        });

    std::thread serverThread([&]() {
        server.start(3968);
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::string request =
        "GET /home HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "Connection: close\r\n\r\n";

    std::string response = sendRequest(request);

    REQUIRE(response.find("200") != std::string::npos);
    REQUIRE(response.find("Benvenuto su /home!") != std::string::npos);

    serverThread.detach();
}