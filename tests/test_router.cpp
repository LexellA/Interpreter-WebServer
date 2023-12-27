#include "../src/lib/router.h"
#include <cassert>
#include <iostream>

using namespace server;

void test_router()
{
    Router router;

    // 添加处理程序
    router.add_handler(HTTPMethod::GET, "/test", [](const HTTPRequest& req, HTTPResponse& res) {
        res.init(HTTPStatus::OK, false, "OK GET", "txt");
    });
    router.add_handler(HTTPMethod::POST, "/test", [](const HTTPRequest& req, HTTPResponse& res) {
        res.init(HTTPStatus::OK, false, "OK POST", "txt");
    });

    // 创建请求和响应对象
    HTTPRequest req1;
    HTTPResponse res1;

    Buffer buffer1(100);
    buffer1.push_back("GET /test HTTP/1.1\r\n\r\n");
    req1.parse(buffer1);

    // 测试GET /test
    router.route(req1, res1);

    Buffer buffer2(100);
    res1.get_response(buffer2);

    assert(std::string(buffer2.data()) == std::string("HTTP/1.1 200 OK\r\nContent-length: 6\r\nContent-type: text/plain\r\nConnection: close\r\n\r\nOK GET"));



    HTTPRequest req2;
    HTTPResponse res2;
    Buffer buffer3(100);
    buffer3.push_back("POST /test HTTP/1.1\r\n\r\n");
    req2.reset();
    req2.parse(buffer3);

    // 测试POST /test
    router.route(req2, res2);

    Buffer buffer4(100);
    res2.get_response(buffer4);
    
    assert(std::string(buffer4.data()) == std::string("HTTP/1.1 200 OK\r\nContent-length: 7\r\nContent-type: text/plain\r\nConnection: close\r\n\r\nOK POST"));


    std::cout << "test_router passed" << std::endl;
}

int main()
{
    test_router();
    return 0;
}