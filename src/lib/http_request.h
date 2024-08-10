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
  enum class RequestState {
    OK,
    INVALID,
    INCOMPLETE
  };
    HTTPRequest();
    ~HTTPRequest();

    void reset();

    RequestState parse(Buffer& buffer);

    HTTPMethod get_method() const { return m_method; }
    HTTPVersion get_version() const { return m_version; }
    const std::string& get_path() const { return m_path; }
    const std::string& get_body() const { return m_body; }
    const std::unordered_map<std::string, std::string>& get_header() const { return m_headers; }
    const std::unordered_map<std::string, std::string>& get_query_params() const { return m_query_params; }
    bool is_keep_alive() const;

    const std::string& get_method_str() const noexcept { return m_method_str_map.at(m_method); }
    const std::string& get_version_str() const noexcept { return m_version_str_map.at(m_version); }

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
    std::unordered_map<std::string, std::string> m_query_params;

    ParseState m_state;

    static const std::unordered_map<std::string_view, HTTPMethod> m_method_map;
    static const std::unordered_map<HTTPMethod, std::string> m_method_str_map;
    static const std::unordered_map<std::string_view, HTTPVersion> m_version_map;
    static const std::unordered_map<HTTPVersion, std::string> m_version_str_map;

    bool parse_request_line(std::string_view request_line);
    bool parse_query_string(std::string_view query_string);
    bool parse_header(std::string_view header);

    std::string decode(std::string_view str);

    HTTPMethod parse_method(std::string_view method);
    HTTPVersion parse_version(std::string_view version);

};

} // namespace server