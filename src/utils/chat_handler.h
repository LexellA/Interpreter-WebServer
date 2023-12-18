#pragma once

#include "../lib/http_request.h"
#include "../lib/http_response.h"



namespace utils
{

class ChatHandler
{
public:
    ChatHandler();
    ~ChatHandler();

    void handle_chat(const server::HTTPRequest& req, server::HTTPResponse& res);

private:


};



}// namespace server