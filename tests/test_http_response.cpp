#include <iostream>
#include <cassert>
#include "../src/http_response.h"


using namespace server;

void test_init()
{
    HTTPResponse response;
    response.init(HTTPStatus::OK, true, "Hello, World!", "text/plain");

    Buffer buffer(100);

    assert(response.has_mmap() == false);
    response.get_response(buffer);
    assert(buffer.available() > 0);
    std::cout<< buffer.data() << std::endl;
}

void test_set_file()
{
    HTTPResponse response;
    response.init(HTTPStatus::OK, true);
    response.set_file("root", "/1.html");

    assert(response.has_mmap() == true);
    std::cout << response.get_content() << std::endl;
}

void test_get_response()
{
    HTTPResponse response;
    response.init(HTTPStatus::OK, true, "Hello, World!", "txt");

    Buffer buffer(100);
    response.get_response(buffer);

    assert(buffer.available() > 0);
    assert(std::string(buffer.data()).find("HTTP/1.1 200 OK") != std::string::npos);
    assert(std::string(buffer.data()).find("Content-length: 13") != std::string::npos);
    assert(std::string(buffer.data()).find("Content-type: text/plain") != std::string::npos);
    assert(std::string(buffer.data()).find("Connection: keep-alive") != std::string::npos);
    assert(std::string(buffer.data()).find("Hello, World!") != std::string::npos);
}


int main()
{
    test_init();
    test_set_file();
    test_get_response();

    std::cout << "HTTPResponse tests passed!" << std::endl;

    return 0;
}