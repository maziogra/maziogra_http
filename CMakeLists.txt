cmake_minimum_required(VERSION 3.28)
project(maziogra_http VERSION 1.0.0)

add_library(
	CoreSocket
	src/CoreSocket.cpp
)

add_library(
	ServerHTTP
	src/HttpRequest.cpp
	src/HttpResponse.cpp
	src/ServerHTTP.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main src/Main.cpp)

if(WIN32)
	target_link_libraries(CoreSocket PRIVATE ws2_32)
	target_link_libraries(ServerHTTP PRIVATE ws2_32)
endif()

target_link_libraries(main PRIVATE CoreSocket ServerHTTP)
