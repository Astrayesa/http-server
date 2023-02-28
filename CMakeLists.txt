cmake_minimum_required(VERSION 3.00)

project(http_server)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(http http.c)
add_library(list list.c)
add_executable(server server.c)
target_link_libraries(
  server 
  http
)


add_executable(http_test http_test.cpp)
target_link_libraries(
  http_test
  http
  GTest::gtest_main
)

add_executable(list_test list_test.cpp)
target_link_libraries(
  list_test
  list
  GTest::gtest_main
)

include(CTest)
add_test(NAME server COMMAND server)
add_test(NAME curl COMMAND curl -s http://localhost:8080/hello)

include(GoogleTest)
gtest_discover_tests(http_test)
gtest_discover_tests(list_test)