
#include <nlohmann/json.hpp>
#include <memory>
#include <cppconn/prepared_statement.h>

#include "login_handler.h"
#include "../lib/log.h"



namespace utils
{

LoginHandler::LoginHandler(std::shared_ptr<server::DBConnectionPool> dbpool)
    : m_dbpool(dbpool)
{   
}

LoginHandler::~LoginHandler()
{
}


void LoginHandler::handle_login(const server::HTTPRequest& req, server::HTTPResponse& res)
{
    nlohmann::json userinfo = nlohmann::json::parse(req.get_body());
    nlohmann::json response;

    std::string username = userinfo["username"];
    std::string password = userinfo["password"];

    bool is_login = false;

    try
    {
        is_login = login(username, password);
    }
    catch (const std::exception& e)
    {
        response["success"] = false;
        response["message"] = "Internal Server Error";
        res.init(server::HTTPStatus::INTERNAL_SERVER_ERROR, false, response.dump(), "json");
        return;
    }


    if (is_login)
    {
        response["success"] = true;
    }
    else
    {
        response["success"] = false;
        response["message"] = "Wrong username or password";
    }

    server::log_info("user: {}, password: {}, LOGIN, result: {}", username, password, is_login);

    res.init(server::HTTPStatus::OK, true, response.dump(), "json");
}

void LoginHandler::handle_register(const server::HTTPRequest& req, server::HTTPResponse& res)
{
    nlohmann::json userinfo = nlohmann::json::parse(req.get_body());
    nlohmann::json response;

    std::string username = userinfo["username"];
    std::string password = userinfo["password"];

    bool is_reg = false;

    try
    {
        is_reg = reg(username, password);
    }
    catch (const std::exception& e)
    {
        response["success"] = false;
        response["message"] = "Internal Server Error";
        res.init(server::HTTPStatus::INTERNAL_SERVER_ERROR, false, response.dump(), "json");
        return;
    }

    if (is_reg)
    {
        response["success"] = true;
    }
    else
    {
        response["success"] = false;
        response["message"] = "Username already exists";
    }

    server::log_info("user: {}, password: {}, REGISTER, result: {}", username, password, is_reg);
    res.init(server::HTTPStatus::OK, true, response.dump(), "json");
}

bool LoginHandler::login(const std::string& username, const std::string& password)
{
    auto conn = m_dbpool->get_connection();
    if (conn == nullptr)
    {
        return false;
    }

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("SELECT * FROM user WHERE username = ? AND password = ?")
        );
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res->next();
    }
    catch (const std::exception& e)
    {
        server::log_error("database error: {}", e.what());
        throw e;
    }

    return true;
}

bool LoginHandler::reg(const std::string& username, const std::string& password)
{
    auto conn = m_dbpool->get_connection();
    if (conn == nullptr)
    {
        return false;
    }

    try
    {
        // 检查用户名是否已经存在
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            conn->prepareStatement("SELECT * FROM user WHERE username = ?")
        );
        checkStmt->setString(1, username);
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());
        if (res->next()) {
            // 如果存在相同的用户名，返回false
            return false;
        }

        // 插入新用户
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO user (username, password) VALUES (?, ?)")
        );
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        pstmt->executeUpdate();
    }
    catch (const std::exception& e)
    {
        server::log_error("database error: {}", e.what());
        throw e;
    }

    return true;
}

} // namespace utils