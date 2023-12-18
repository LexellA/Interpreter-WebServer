
#include <nlohmann/json.hpp>


#include "chat_handler.h"



namespace utils
{

ChatHandler::ChatHandler()
{
}

ChatHandler::~ChatHandler()
{
}


void ChatHandler::handle_chat(const server::HTTPRequest& req, server::HTTPResponse& res)
{
    auto &query = req.get_query_params();
    std::string msg = query.at("q");
    nlohmann::json response;
    nlohmann::json content;

    content["text"] = "你" + msg + "个√";
    response["type"] = "text";
    response["content"] = content;

    res.init(server::HTTPStatus::OK, true, response.dump(), "json");
}

} // namespace utils