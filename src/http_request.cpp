
#include <vector>

#include "http_request.h"
#include "log.h"

namespace server
{

const std::unordered_map<std::string_view, HTTPMethod> HTTPRequest::m_method_map = 
{
    {"GET", HTTPMethod::GET},
    {"POST", HTTPMethod::POST},
    {"PUT", HTTPMethod::PUT},
    {"DELETE", HTTPMethod::DELETE},
    {"HEAD", HTTPMethod::HEAD},
    {"CONNECT", HTTPMethod::CONNECT},
    {"OPTIONS", HTTPMethod::OPTIONS},
    {"TRACE", HTTPMethod::TRACE},
    {"PATCH", HTTPMethod::PATCH}
};

const std::unordered_map<std::string_view, HTTPVersion> HTTPRequest::m_version_map = 
{
    {"HTTP/1.0", HTTPVersion::HTTP_10},
    {"HTTP/1.1", HTTPVersion::HTTP_11},
    {"HTTP/2.0", HTTPVersion::HTTP_20}
};


HTTPRequest::HTTPRequest()
    : m_method(HTTPMethod::UNKNOWN), m_version(HTTPVersion::UNKNOWN)
    , m_state(ParseState::REQUEST_LINE)
{
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::reset()
{
    m_method = HTTPMethod::UNKNOWN;
    m_version = HTTPVersion::UNKNOWN;
    m_path.clear();
    m_body.clear();
    m_headers.clear();
    m_state = ParseState::REQUEST_LINE;
}

bool HTTPRequest::parse(Buffer& buffer)
{
    const std::string CRLF = "\r\n";
    size_t pos = 0;
    std::string_view request(buffer.data(), buffer.available());

    while (m_state != ParseState::FINISHED)
    {
        size_t line_end = request.find(CRLF, pos);
        if(m_state == ParseState::BODY)
        {
            line_end = request.size();
        }
        if (line_end == std::string::npos)
            return false;
        size_t len = line_end - pos;
        std::string_view line(request.data() + pos, len);

        switch (m_state)
        {
        case ParseState::REQUEST_LINE:
            if (!parse_request_line(line))
            {
                log_error("parse request line failed, request: \n{}\nerror line: {}", request, line);
                return false;
            }
            break;
        case ParseState::HEADER:
            if (!parse_header(line))
            {
                log_error("parse header failed, header, request: \n{}\nerror header: {}", request, line);
                return false;
            }
            break;
        case ParseState::BODY:
            m_body = request.substr(pos);
            m_state = ParseState::FINISHED;
            break;
        default:
            break;
        }

        pos += len + CRLF.size();
    }

    buffer.pop_front(pos);
    return true;
}

bool HTTPRequest::parse_request_line(std::string_view request_line)
{
    if(request_line.empty())
        return false;

    std::vector<std::string_view> parts;
    size_t pos = 0, end = 0;
    while ((end = request_line.find(' ', pos)) != std::string_view::npos)
    {
        parts.push_back(request_line.substr(pos, end - pos));
        pos = end + 1;
    }
    parts.push_back(request_line.substr(pos));

    if (parts.size() != 3)
        return false;

    m_method = parse_method(parts[0]);

    m_version = parse_version(parts[2]);

    m_path = parts[1];

    m_state = ParseState::HEADER;

    if(m_method == HTTPMethod::UNKNOWN || m_version == HTTPVersion::UNKNOWN)
        return false;

    return true;
}

bool HTTPRequest::parse_header(std::string_view header)
{
    if(header.empty())
    {
        m_state = ParseState::BODY;
        return true;
    }

    size_t pos = header.find(':');
    if(pos == std::string_view::npos)
        return false;
    
    std::string_view key(header.substr(0, pos));
    std::string_view value(header.substr(pos + 2));

    m_headers[std::string(key)] = std::string(value);
    return true;
}

HTTPMethod HTTPRequest::parse_method(std::string_view method)
{
    auto it = m_method_map.find(method);
    if(it != m_method_map.end())
        return it->second;
    else
        return HTTPMethod::UNKNOWN;
}

HTTPVersion HTTPRequest::parse_version(std::string_view version)
{
    auto it = m_version_map.find(version);
    if(it != m_version_map.end())
        return it->second;
    else
        return HTTPVersion::UNKNOWN;
}

} // namespace server