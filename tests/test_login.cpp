#include "../src/utils/login_handler.h"
#include "../src/lib/log.h"
#include <cassert>
#include <iostream>

using namespace utils;
using namespace server;

void testLoginHandler()
{
    // 创建一个模拟的数据库连接池和会话对象
    auto dbpool = std::make_shared<server::DBConnectionPool>("localhost", "root", "18523873534", "chatbot", 3306, 8);
    auto session = std::make_shared<utils::Session>();

    // 创建一个LoginHandler对象
    LoginHandler login(dbpool, session);

    // 创建一个模拟的HTTP请求和响应
    server::HTTPRequest req;
    server::HTTPResponse res;

    std::string httpRequest =
        "POST /login HTTP/1.1\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 45\r\n"
        "Content-Type: application/json\r\n"
        "Cookie: x-hng=lang=zh-CN; session_id=0\r\n"
        "Host: localhost:3411\r\n"
        "Origin: http://localhost:3411\r\n"
        "Referer: http://localhost:3411/login.html\r\n"
        "Sec-Fetch-Dest: empty\r\n"
        "Sec-Fetch-Mode: cors\r\n"
        "Sec-Fetch-Site: same-origin\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n"
        "sec-ch-ua: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"120\", \"Microsoft Edge\";v=\"120\"\r\n"
        "sec-ch-ua-mobile: ?0\r\n"
        "sec-ch-ua-platform: \"Windows\"\r\n\r\n"
        "{\"username\":\"2021210959\",\"password\":\"123456\"}";

    Buffer buffer(1024);
    Buffer buffer2(1024);
    buffer.push_back(httpRequest);

    req.parse(buffer);

    login.handle_login(req, res);

    res.get_response(buffer2);

    std::string httpResponse = 
        "HTTP/1.1 200 OK\r\n"
        "Content-length: 56\r\n"
        "Content-type: application/json\r\n"
        "Keep-Alive: timeout=120, max=6\r\n"
        "Connection: keep-alive\r\n\r\n"
        "{\"message\":\"Wrong username or password\",\"success\":false}";
    

    assert(std::string(buffer2.data()) == httpResponse);


    std::cout << "testLoginHandler passed" << std::endl;
}

int main()
{
    server::Log::get_instance().init(Log::Level::DEBUG, "test_login.log");
    testLoginHandler();
    return 0;
}