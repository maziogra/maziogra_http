cmake_minimum_required(VERSION 3.22)
project(maziogra_http VERSION 1.0 LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(maziogra_http
    main.cpp
    src/socket/BaseSocket.cpp
    src/server/ServerHTTP.cpp
    src/server/HttpRequest.cpp
    src/server/ServerThread.cpp
        include/socket/SecureSocket.h
        src/socket/SecureSocket.cpp
)

target_sources(maziogra_http PRIVATE
    include/socket/Socket.h
    include/socket/BaseSocket.h
    include/server/ServerHTTP.h
    include/server/HttpRequest.h
    include/server/HttpResponse.h
    include/server/ServerThread.h
)

find_package(OpenSSL REQUIRED)

if(OpenSSL_FOUND)
    target_include_directories(maziogra_http PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(maziogra_http PRIVATE OpenSSL::SSL OpenSSL::Crypto)
else()
    message(FATAL_ERROR "OpenSSL non trovato")
endif()

