
#include <vector>

#include "http_request.h"
#include "log.h"

namespace server
{

HTTPRequest::HTTPRequest()
    : m_method(HTTPMethod::UNKNOWN), m_version(HTTPVersion::UNKNOWN)
    , m_state(ParseState::REQUEST_LINE)
{
}

HTTPRequest::~HTTPRequest()
{
}

bool HTTPRequest::parse(const std::string &request)
{
    const std::string CRLF = "\r\n";
    size_t pos = 0;
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

    if (parts[0] == "GET")
        m_method = HTTPMethod::GET;
    else if (parts[0] == "POST")
        m_method = HTTPMethod::POST;
    else if (parts[0] == "HEAD")
        m_method = HTTPMethod::HEAD;
    else if (parts[0] == "PUT")
        m_method = HTTPMethod::PUT;
    else if (parts[0] == "DELETE")
        m_method = HTTPMethod::DELETE;
    else if (parts[0] == "CONNECT")
        m_method = HTTPMethod::CONNECT;
    else if (parts[0] == "OPTIONS")
        m_method = HTTPMethod::OPTIONS;
    else if (parts[0] == "TRACE")
        m_method = HTTPMethod::TRACE;
    else if (parts[0] == "PATCH")
        m_method = HTTPMethod::PATCH;
    else
    {
        m_method = HTTPMethod::UNKNOWN;
        return false;
    }

    if (parts[2] == "HTTP/1.0")
        m_version = HTTPVersion::HTTP_10;
    else if (parts[2] == "HTTP/1.1")
        m_version = HTTPVersion::HTTP_11;
    else if (parts[2] == "HTTP/2.0")
        m_version = HTTPVersion::HTTP_20;
    else
    {
        m_version = HTTPVersion::UNKNOWN;
        return false;
    }

    m_path = parts[1];

    m_state = ParseState::HEADER;
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

} // namespace server