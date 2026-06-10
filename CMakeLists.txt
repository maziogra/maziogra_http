cmake_minimum_required(VERSION 3.22)
project(maziogra_http VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(maziogra_core
    src/socket/BaseSocket.cpp
    src/server/ServerHTTP.cpp
    
    src/server/ServerThread.cpp
    src/socket/SecureSocket.cpp
    src/server/routes/Router.cpp
 "src/server/HttpRequestParser.cpp")

target_include_directories(maziogra_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(OpenSSL REQUIRED)

target_link_libraries(maziogra_core PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
)

if (WIN32)
    target_link_libraries(maziogra_core PUBLIC ws2_32)
endif()

add_executable(maziogra_http main.cpp)

target_link_libraries(maziogra_http PRIVATE maziogra_core)

add_executable(maziogra_tests
    tests/test_parser.cpp
 "tests/test_request.cpp")

find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(maziogra_tests PRIVATE
    maziogra_core
    Catch2::Catch2WithMain
)

enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(maziogra_tests)