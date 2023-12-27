#include "server.h"

namespace server
{

Server::Server(int port, int trig_mode, int timeout, bool is_linger, int thread_num, const std::string &srcdir,
                const std::string sql_host, int sql_port, const std::string &sql_user, const std::string &sql_password, const std::string &db_name, int max_conn,
                int timer_interval, Log::Level log_level, const std::string &scripts_src)
    : ServerBase(port, trig_mode, timeout, is_linger, thread_num, srcdir, sql_host, sql_port, sql_user, sql_password, db_name, max_conn, timer_interval, log_level),
        m_session(new utils::Session()),
        m_login_handler(new utils::LoginHandler(get_dbpool(), m_session)),
        m_chat_handler(new utils::ChatHandler(m_session, scripts_src))
{

    // 添加路由
    add_handler(HTTPMethod::POST, "/login",
                std::bind(&utils::LoginHandler::handle_login, m_login_handler.get(), std::placeholders::_1, std::placeholders::_2));

    add_handler(HTTPMethod::POST, "/register",
                std::bind(&utils::LoginHandler::handle_register, m_login_handler.get(), std::placeholders::_1, std::placeholders::_2));

    add_handler(HTTPMethod::GET, "/chat/send",
                std::bind(&utils::ChatHandler::handle_chat, m_chat_handler.get(), std::placeholders::_1, std::placeholders::_2));
}

Server::~Server()
{
}

void Server::default_handler(const HTTPRequest &req, HTTPResponse &res)
{
    // 获取cookie中的session_id
    std::string cookie;
    try{
        cookie = req.get_header().at("Cookie");
    }
    catch(const std::exception& e)
    {
        cookie = "";
    }
    std::string session_id_key = "session_id=";
    size_t start_pos = cookie.find(session_id_key);
    uint32_t ssid = -1;
    if (start_pos != std::string::npos)
    {
        start_pos += session_id_key.length();
        size_t end_pos = cookie.find(';', start_pos); 
        if (end_pos == std::string::npos)
        {
            end_pos = cookie.length(); 
        }
        std::string session_id = cookie.substr(start_pos, end_pos - start_pos);
        try
        {
            ssid = std::stoi(session_id);
        }
        catch (const std::exception &e)
        {
            ssid = -1;
        }
    }

    // 根据session_id获取对应的解释器
    auto interpreter = m_session->get_interpreter(ssid);

    // 如果没有解释器，且请求的不是登录页面，则跳转到登录页面（未登录状态下只能访问登录页面）
    if (interpreter == nullptr)
    {
        if (req.get_path() != "/login.html" && req.get_path() != "/js/login.js")
        {
            res.init(server::HTTPStatus::FOUND, true);
            res.add_header("Location", "/login.html");
            return;
        }
    }
    ServerBase::default_handler(req, res);
}

}