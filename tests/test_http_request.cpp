#include <iostream>
#include <cassert>
#include <string>
#include "../src/http_request.h"
#include "../src/buffer.h"

using namespace server;

void test_parse_request()
{
    std::string request = "POST /login HTTP/1.1\r\n"
                          "Host: 127.0.0.1:10238\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0\r\n"
                          "Accept: */*\r\n"
                          "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Content-Type: application/json\r\n"
                          "Content-Length: 31\r\n"
                          "Origin: http://127.0.0.1:10238\r\n"
                          "Connection: keep-alive\r\n"
                          "Referer: http://127.0.0.1:10238/login.html\r\n"
                          "Cookie: x-hng=lang=zh-CN\r\n"
                          "Sec-Fetch-Dest: empty\r\n"
                          "Sec-Fetch-Mode: cors\r\n"
                          "Sec-Fetch-Site: same-origin\r\n"
                          "\r\n"
                          "{\"username\":\"1\",\"password\":\"1\"}";

    Buffer buffer(1024);
    buffer.push_back(request);
    HTTPRequest http_request;
    bool result = http_request.parse(buffer);

    // Verify the parsing result
    assert(result == true);
    assert(http_request.get_method() == HTTPMethod::POST);
    assert(http_request.get_version() == HTTPVersion::HTTP_11);
    assert(http_request.get_path() == "/login");
    assert(http_request.get_header().at("Host") == "127.0.0.1:10238");
    assert(http_request.get_header().at("User-Agent") == "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0");
    assert(http_request.get_header().at("Accept") == "*/*");
    assert(http_request.get_header().at("Accept-Language") == "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    assert(http_request.get_header().at("Accept-Encoding") == "gzip, deflate, br");
    assert(http_request.get_header().at("Content-Type") == "application/json");
    assert(http_request.get_header().at("Content-Length") == "31");
    assert(http_request.get_header().at("Origin") == "http://127.0.0.1:10238");
    assert(http_request.get_header().at("Connection") == "keep-alive");
    assert(http_request.get_header().at("Referer") == "http://127.0.0.1:10238/login.html");
    assert(http_request.get_header().at("Cookie") == "x-hng=lang=zh-CN");
    assert(http_request.get_header().at("Sec-Fetch-Dest") == "empty");
    assert(http_request.get_header().at("Sec-Fetch-Mode") == "cors");
    assert(http_request.get_header().at("Sec-Fetch-Site") == "same-origin");
    assert(http_request.get_body() == "{\"username\":\"1\",\"password\":\"1\"}");
}

void test_parse_request_invalid()
{
    std::string request = "POST /api/users HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Content-Type: application/json\r\n";

    Buffer buffer(1024);
    buffer.push_back(request);
    HTTPRequest http_request;
    bool result = http_request.parse(buffer);

    // Verify the parsing result
    assert(result == false);
}

int main()
{
    test_parse_request();
    test_parse_request_invalid();

    std::cout << "HTTP request tests passed!" << std::endl;

    return 0;
}