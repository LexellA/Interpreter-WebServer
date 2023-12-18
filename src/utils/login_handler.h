#pragma once

#include "../lib/http_request.h"
#include "../lib/http_response.h"
#include "../lib/db_connectionpool.h"

namespace utils
{

class LoginHandler
{
public:
    LoginHandler(std::shared_ptr<server::DBConnectionPool> dbpool);
    ~LoginHandler();

    void handle_login(const server::HTTPRequest& req, server::HTTPResponse& res);
    void handle_register(const server::HTTPRequest& req, server::HTTPResponse& res);
private:
    bool login(const std::string& username, const std::string& password);
    bool reg(const std::string& username, const std::string& password);

    std::shared_ptr<server::DBConnectionPool> m_dbpool;

};


}