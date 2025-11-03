cmake_minimum_required(VERSION 3.30)
project(maziogra_http)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(maziogra_http main.cpp
        include/socket/Socket.h
        src/socket/BaseSocket.cpp
        include/socket/BaseSocket.h
        src/Server/ServerHTTP.cpp
        include/server/ServerHTTP.h
)

target_link_libraries(maziogra_http PRIVATE ws2_32)