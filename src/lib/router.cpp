
#include <stdexcept>
#include <string>

#include "router.h"

namespace server
{

Router::Router()
    : m_handlers({})
{}

Router::~Router()
{}

void Router::add_handler(HTTPMethod method, const std::string& path, handler h)
{
    m_handlers[std::make_pair(method, path)] = h;
}

void Router::route(const HTTPRequest &req, HTTPResponse &res)
{
    auto it = m_handlers.find(std::make_pair(req.get_method(), req.get_path()));
    if (it != m_handlers.end())
    {
        it->second(req, res);
    }
    else
    {
        it = m_handlers.find(std::make_pair(req.get_method(), "default"));
        if (it != m_handlers.end())
        {
            it->second(req, res);
        }
        else
        {
            throw std::runtime_error("No handler for method: " + req.get_method_str() + ", path: " + req.get_path());
        }
    }
}




}// namespace server