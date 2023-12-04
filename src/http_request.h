#pragma once

#include <string>
#include <unordered_map>


#include "http_types.h"
#include "buffer.h"

namespace server
{

class HTTPRequest
{
public:
    HTTPRequest();
    ~HTTPRequest();

    void reset();

    bool parse(Buffer& buffer);

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

    static const std::unordered_map<std::string_view, HTTPMethod> m_method_map;
    static const std::unordered_map<std::string_view, HTTPVersion> m_version_map;

    bool parse_request_line(std::string_view request_line);
    bool parse_header(std::string_view header);

    HTTPMethod parse_method(std::string_view method);
    HTTPVersion parse_version(std::string_view version);

};

} // namespace server