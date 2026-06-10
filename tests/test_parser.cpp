#include <catch2/catch_test_macros.hpp>
#include <server/HttpRequest.h>
#include <server/HttpRequestParser.h>

using namespace maziogra_http;
HttpRequestParser parser;

TEST_CASE("BasicRequest") {
    std::string raw = "GET /api/users HTTP/1.1\r\n"
        "Host: example.com\r\n";
    HttpRequest req;
	parser.parse(raw, req);

    REQUIRE(req.getMethod() == "GET");
    REQUIRE(req.getPath() == "/api/users");
    REQUIRE(req.getVersion() == "HTTP/1.1");
}


TEST_CASE("HttpRequest parses headers correctly") {
    std::string raw =
        "GET /api/users HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Authorization: Bearer TOKEN123\r\n"
        "X-Custom: value\r\n"
        "\r\n";

    HttpRequest req;
    parser.parse(raw, req);

    auto headers = req.getHeaders();

    REQUIRE(headers.size() == 3);

    REQUIRE(headers.at("Host") == "example.com");
    REQUIRE(headers.at("Authorization") == "Bearer TOKEN123");
    REQUIRE(headers.at("X-Custom") == "value");
}

TEST_CASE("POST request parsing")
{
    std::string raw =
        "POST /submit HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: application/json\r\n"
        "\r\n";

    HttpRequest req;
    parser.parse(raw, req);

    REQUIRE(req.getMethod() == "POST");
    REQUIRE(req.getPath() == "/submit");
    REQUIRE(req.getVersion() == "HTTP/1.1");

    auto headers = req.getHeaders();
    REQUIRE(headers.at("Content-Type") == "application/json");
}

TEST_CASE("Request with no headers")
{
    std::string raw =
        "GET / HTTP/1.1\r\n"
        "\r\n";

    HttpRequest req;
    parser.parse(raw, req);

    REQUIRE(req.getMethod() == "GET");
    REQUIRE(req.getPath() == "/");
    REQUIRE(req.getHeaders().empty());
}

TEST_CASE("Duplicate headers")
{
    std::string raw =
        "GET / HTTP/1.1\r\n"
        "X-Test: one\r\n"
        "X-Test: two\r\n"
        "\r\n";

    HttpRequest req;
    parser.parse(raw, req);

    auto headers = req.getHeaders();

    REQUIRE(headers.at("X-Test") == "two");
}

TEST_CASE("Malformed request missing HTTP version")
{
    std::string raw =
        "GET /api/users\r\n"
        "Host: example.com\r\n"
        "\r\n";
    HttpRequest req;
    
    REQUIRE_THROWS_AS(parser.parse(raw, req), std::invalid_argument);
}

TEST_CASE("Empty request")
{
    HttpRequest req;
    
    REQUIRE_THROWS_AS(parser.parse("", req), std::invalid_argument);
}