cmake_minimum_required(VERSION 3.00)

project(http_server)

add_executable(server server.c)
include(CTest)
add_test(NAME server COMMAND server)
add_test(NAME curl COMMAND curl -s http://localhost:8080/hello)