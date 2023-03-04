#include "http.h"
#include "list.h"
#include <gtest/gtest.h>

// parsing request test

TEST(ParseRequest, SuccessCase)
{
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: testcase\r\n"
        "\r\n";
    
    http_request req;
    ASSERT_EQ(parse_request((char*) request.c_str(), &req), 0);
    EXPECT_STREQ(req.method, "GET");
    EXPECT_STREQ(req.uri, "/");
    EXPECT_STREQ(req.version, "HTTP/1.1");
    EXPECT_STREQ(get_node(&req.header, (char*) "Host")->value, "localhost:8080");
    EXPECT_STREQ(get_node(&req.header, (char*) "User-Agent")->value, "testcase");
    EXPECT_EQ(strlen(req.body), 0);
    clear_list(&req.header);
}

TEST(ParseRequest, FailCase)
{
    // malformatted request
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: testcase\r\n";
    
    http_request req;

    ASSERT_EQ(parse_request((char*) request.c_str(), &req), -1);
}

TEST(ParseRequest, UnsupportCase){
    std::string request =
    "HEAD / HTTP/1.1\r\n"
    "Host: localhost:8080\r\n"
    "User-Agent: testcase\r\n"
    "\r\n";
    http_request req;

    ASSERT_EQ(parse_request((char*) request.c_str(), &req), 0);
    EXPECT_STREQ(req.method, "HEAD");
    EXPECT_STREQ(req.uri, "/");
    EXPECT_STREQ(req.version, "HTTP/1.1");
    clear_list(&req.header);
}

TEST(ParseHeader, SuccessCase)
{
    std::string header =
        "Host: localhost:8080\r\n"
        "User-Agent: testcase\r\n";
    
    List header_list;
    // init_list(&header_list);
    ASSERT_EQ(parse_header((char*) header.c_str(), &header_list), 0);
    EXPECT_STREQ(get_node(&header_list, (char*) "Host")->value, "localhost:8080");
    EXPECT_STREQ(get_node(&header_list, (char*) "User-Agent")->value, "testcase");
    clear_list(&header_list);
}