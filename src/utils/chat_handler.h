#pragma once

#include "../lib/http_request.h"
#include "../lib/http_response.h"
#include "session.h"



namespace utils
{

class ChatHandler
{
public:
    ChatHandler(std::shared_ptr<utils::Session> session, const std::string& src);
    ~ChatHandler();

    void handle_chat(const server::HTTPRequest& req, server::HTTPResponse& res);

private:
    std::shared_ptr<utils::Session> m_session;
    std::string m_src;
};



}// namespace server