
#include <vector>
#include <sstream>

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

const std::unordered_map<HTTPMethod, std::string> HTTPRequest::m_method_str_map = 
{
    {HTTPMethod::GET, "GET"},
    {HTTPMethod::POST, "POST"},
    {HTTPMethod::PUT, "PUT"},
    {HTTPMethod::DELETE, "DELETE"},
    {HTTPMethod::HEAD, "HEAD"},
    {HTTPMethod::CONNECT, "CONNECT"},
    {HTTPMethod::OPTIONS, "OPTIONS"},
    {HTTPMethod::TRACE, "TRACE"},
    {HTTPMethod::PATCH, "PATCH"},
    {HTTPMethod::UNKNOWN, "UNKNOWN"}
};

const std::unordered_map<std::string_view, HTTPVersion> HTTPRequest::m_version_map = 
{
    {"HTTP/1.0", HTTPVersion::HTTP_10},
    {"HTTP/1.1", HTTPVersion::HTTP_11},
    {"HTTP/2.0", HTTPVersion::HTTP_20},
};

const std::unordered_map<HTTPVersion, std::string> HTTPRequest::m_version_str_map = 
{
    {HTTPVersion::HTTP_10, "HTTP/1.0"},
    {HTTPVersion::HTTP_11, "HTTP/1.1"},
    {HTTPVersion::HTTP_20, "HTTP/2.0"},
    {HTTPVersion::UNKNOWN, "UNKNOWN"}
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
    m_query_params.clear();
    m_state = ParseState::REQUEST_LINE;
}

bool HTTPRequest::parse(Buffer& buffer)
{
    const std::string CRLF = "\r\n";
    size_t pos = 0;
    std::string_view request(buffer.data(), buffer.available());

    // 使用状态机解析请求
    while (m_state != ParseState::FINISHED)
    {
        size_t line_end = request.find(CRLF, pos);
        size_t len = 0;
        std::string_view line;
        switch (m_state)
        {
        case ParseState::REQUEST_LINE:
            if(line_end == std::string::npos)
            {
                return false;
            }
            len = line_end - pos;
            line = std::string_view(request.data() + pos, len);
            
            
            if (!parse_request_line(line))
            {
                log_error("parse request line failed, request: \n{}\nerror line: {}", request, line);
                return false;
            }
            break;
        case ParseState::HEADER:
            if(line_end == std::string::npos)
            {
                return false;
            }
            len = line_end - pos;
            line = std::string_view(request.data() + pos, len);

            if (!parse_header(line))
            {
                log_error("parse header failed, header, request: \n{}\nerror header: {}", request, line);
                return false;
            }
            break;
        case ParseState::BODY:
            if(m_headers.count("Content-Length") == 0)
            {
                m_state = ParseState::FINISHED;
                pos -= CRLF.size();
                break;
            }
            len = std::stoi(m_headers["Content-Length"]);
            m_body = std::string(request.data() + pos, len);
            m_state = ParseState::FINISHED;
            pos -= CRLF.size();
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
    pos = m_path.find('?');
    if(pos != std::string_view::npos)
    {
        std::string_view query_string(m_path);
        parse_query_string(query_string.substr(pos + 1));
        m_path = m_path.substr(0, pos);
    }

    m_state = ParseState::HEADER;

    if(m_method == HTTPMethod::UNKNOWN || m_version == HTTPVersion::UNKNOWN)
        return false;

    return true;
}

bool HTTPRequest::parse_query_string(std::string_view query_string)
{
    // 解析请求参数
    if(query_string.empty())
        return false;

    size_t pos = 0, end = 0;
    while ((end = query_string.find('&', pos)) != std::string_view::npos)
    {
        std::string_view param = query_string.substr(pos, end - pos);
        size_t equal_pos = param.find('=');
        if(equal_pos == std::string_view::npos)
            return false;
        std::string_view key = param.substr(0, equal_pos);
        std::string_view value = param.substr(equal_pos + 1);
        m_query_params[std::string(key)] = decode(value);
        pos = end + 1;
    }
    std::string_view param = query_string.substr(pos);
    size_t equal_pos = param.find('=');
    if(equal_pos == std::string_view::npos)
        return false;
    std::string_view key = param.substr(0, equal_pos);
    std::string_view value = param.substr(equal_pos + 1);
    m_query_params[std::string(key)] = decode(value);

    return true;
}

bool HTTPRequest::parse_header(std::string_view header)
{
    // 解析请求头
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

std::string HTTPRequest::decode(std::string_view str)
{
    // 解码url
    std::string result;
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] == '%' && i + 2 < str.size())
        {
            int value;
            std::istringstream iss(std::string(str.substr(i + 1, 2)));
            if(iss >> std::hex >>value)
            {
                result += static_cast<char>(value);
                i += 2;
            }
            else
            {
                result += '%';
            }
        }
        else if(str[i] == '+')
        {
            result += ' ';
        }
        else
        {
            result += str[i];
        }
    }
    return result;
}

HTTPMethod HTTPRequest::parse_method(std::string_view method)
{
    // 解析请求方法
    auto it = m_method_map.find(method);
    if(it != m_method_map.end())
        return it->second;
    else
        return HTTPMethod::UNKNOWN;
}

HTTPVersion HTTPRequest::parse_version(std::string_view version)
{ 
    // 解析http版本
    auto it = m_version_map.find(version);
    if(it != m_version_map.end())
        return it->second;
    else
        return HTTPVersion::UNKNOWN;
}

bool HTTPRequest::is_keep_alive() const
{
    if (get_version() == HTTPVersion::HTTP_10)
    {
        return m_headers.find("Connection") != m_headers.end() && m_headers.at("Connection") == "keep-alive";
    }
    else
    {
        return m_headers.find("Connection") == m_headers.end() || m_headers.at("Connection") != "close";
    }
    return false;
}

} // namespace server