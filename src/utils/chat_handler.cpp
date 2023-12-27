
#include <nlohmann/json.hpp>


#include "chat_handler.h"
#include "../lib/log.h"



namespace utils
{

ChatHandler::ChatHandler(std::shared_ptr<utils::Session> session, const std::string& src)
    : m_session(session), m_src(src)
{
}

ChatHandler::~ChatHandler()
{
}


void ChatHandler::handle_chat(const server::HTTPRequest& req, server::HTTPResponse& res)
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

    // 获取请求参数
    auto &query = req.get_query_params();
    std::string msg = query.at("q");
    nlohmann::json response;
    nlohmann::json content;

    
    if(interpreter == nullptr)
    {
        content["text"] = "您还没有登录";
    }
    else
    {
        server::log_info("recv msg: {} from session {}", msg, ssid);
        if (!interpreter->is_running())
        {
            interpreter->parse(m_src);
            interpreter->run();
        }
        interpreter->input(msg);
        interpreter->continue_run();
        content["text"] = interpreter->output();
        server::log_info("send msg: {} to session {}", interpreter->output(), ssid);

        if(!interpreter->is_running())
        {
            m_session->remove_ssid(ssid);
        }
    }

    response["type"] = "text";
    response["content"] = content;
    res.init(server::HTTPStatus::OK, true, response.dump(), "json");
}

} // namespace utils