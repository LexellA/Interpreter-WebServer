#include <iostream>
#include <cassert>
#include "../src/http_response.h"
#include "../src/buffer.h"

using namespace server;

void test()
{
    HTTPResponse response(HTTPStatus::FORBIDDEN, "root", "/404.html", true);
    Buffer buffer(1024);
    response.get_header(buffer);
    std::cout << buffer.data();
    std::cout << response.get_content();
    std::cout << response.get_content_length();
}
int main()
{
    test();

    std::cout << "HTTPResponse tests passed!" << std::endl;

    return 0;
}