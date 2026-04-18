cmake_minimum_required(VERSION 3.22)
project(maziogra_http VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(maziogra_http
    main.cpp
    src/socket/BaseSocket.cpp
    src/server/ServerHTTP.cpp
    src/server/HttpRequest.cpp
    src/server/ServerThread.cpp
    src/socket/SecureSocket.cpp
    src/server/routes/Router.cpp
    )

target_include_directories(maziogra_http PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if (WIN32)
    target_link_libraries(maziogra_http PRIVATE ws2_32)
    target_compile_definitions(maziogra_http PRIVATE WIN32_LEAN_AND_MEAN)
endif()

find_package(OpenSSL REQUIRED)

target_link_libraries(maziogra_http PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)