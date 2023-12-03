#include <iostream>
#include <cassert>
#include <string>
#include "../src/http_request.h"

using namespace server;

void test_parse_request()
{
    std::string request = "GET /api/users HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Content-Type: application/json\r\n"
                          "\r\n"
                          "{\"name\": \"John\", \"age\": 30}";

    HTTPRequest http_request;
    bool result = http_request.parse(request);

    // Verify the parsing result
    assert(result == true);
    assert(http_request.get_method() == HTTPMethod::GET);
    assert(http_request.get_version() == HTTPVersion::HTTP_11);
    assert(http_request.get_path() == "/api/users");
    assert(http_request.get_header().at("Host") == "localhost");
    assert(http_request.get_header().at("Content-Type") == "application/json");
    assert(http_request.get_body() == "{\"name\": \"John\", \"age\": 30}");
}

void test_parse_request_invalid()
{
    std::string request = "POST /api/users HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Content-Type: application/json\r\n";

    HTTPRequest http_request;
    bool result = http_request.parse(request);

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