#pragma once

#include <functional>
#include <unordered_map>
#include <string>

#include "http_types.h"
#include "http_request.h"
#include "http_response.h"


namespace server
{

class Router
{
public:
    using handler = std::function<void(const HTTPRequest&, HTTPResponse&)>;
    using key = std::pair<HTTPMethod, std::string>;

    Router();
    ~Router();

    // 添加handler
    void add_handler(HTTPMethod method, const std::string& path, handler h);

    // 调用对应的handler
    void route(const HTTPRequest &req, HTTPResponse &res);

private:
    struct hash
    {
        size_t operator()(const key& k) const
        {
            return std::hash<int>()((int)k.first) ^ std::hash<std::string>()(k.second);
        }
    };// method和path的hash函数
    
    std::unordered_map<key, handler, hash> m_handlers;
    
};

} // namespace server