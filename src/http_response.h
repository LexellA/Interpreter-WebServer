#pragma once

#include <string>
#include <unordered_map>

#include "http_types.h"

namespace server
{

class HTTPResponse
{
public:
    HTTPResponse();
    ~HTTPResponse();

    void init();

private:

    static const std::unordered_map<HTTPStatus, std::string> m_status_map;

    HTTPStatus m_status;
    std::string m_src;
    std::string m_path;
    

};

} // namespace server