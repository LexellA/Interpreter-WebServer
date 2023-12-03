#pragma once

#include <string>
#include <unordered_map>


#include "http_types.h"

namespace server
{

class HTTPRequest
{
public:
    HTTPRequest();
    ~HTTPRequest();

    bool parse(const std::string &request);

    HTTPMethod get_method() const { return m_method; }
    HTTPVersion get_version() const { return m_version; }
    const std::string& get_path() const { return m_path; }
    const std::string& get_body() const { return m_body; }
    const std::unordered_map<std::string, std::string>& get_header() const { return m_headers; }

private:
    enum class ParseState
    {
        REQUEST_LINE,
        HEADER,
        BODY,
        FINISHED
    };

    HTTPMethod m_method;
    HTTPVersion m_version;
    std::string m_path;
    std::string m_body;
    std::unordered_map<std::string, std::string> m_headers;

    ParseState m_state;

    bool parse_request_line(std::string_view request_line);
    bool parse_header(std::string_view header);

};

} // namespace server