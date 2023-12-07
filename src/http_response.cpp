
#include <sys/mman.h>
#include <fcntl.h>

#include "http_response.h"

namespace server
{

const std::unordered_map<HTTPStatus, std::string> HTTPResponse::m_status_map =
{
    {HTTPStatus::UNKNOWN, "Unknown"},
    {HTTPStatus::OK, "Ok"},
    {HTTPStatus::NO_CONTENT, "No Content"},
    {HTTPStatus::PARTIAL_CONTENT, "Partial Content"},
    {HTTPStatus::MOVED_PERMANENTLY, "Moved Permanently"},
    {HTTPStatus::FOUND, "Found"},
    {HTTPStatus::SEE_OTHER, "See Other"},
    {HTTPStatus::NOT_MODIFIED, "Not Modified"},
    {HTTPStatus::TEMPORARY_REDIRECT, "Temporary Redirect"},
    {HTTPStatus::BAD_REQUEST, "Bad Request"},
    {HTTPStatus::UNAUTHORIZED, "Unauthorized"},
    {HTTPStatus::FORBIDDEN, "Forbidden"},
    {HTTPStatus::NOT_FOUND, "Not Found"},
    {HTTPStatus::INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {HTTPStatus::NOT_IMPLEMENTED, "Not Implemented"}
};

const std::unordered_map<std::string, std::string> HTTPResponse::m_content_type_map =
{
    {".html",       "text/html"},
    {".xml",        "text/xml"},
    {".xhtml",      "application/xhtml+xml"},
    {".txt",        "text/plain"},
    {".rtf",        "application/rtf"},
    {".pdf",        "application/pdf"},
    {".word",       "application/msword"},
    {".png",        "image/png"},
    {".gif",        "image/gif"},
    {".jpg",        "image/jpeg"},
    {".jpeg",       "image/jpeg"},
    {".au",         "audio/basic"},
    {".mpeg",       "video/mpeg"},
    {".mpg",        "video/mpeg"},
    {".avi",        "video/x-msvideo"},
    {".gz",         "application/x-gzip"},
    {".tar",        "application/x-tar"},
    {".css",        "text/css"},
    {".js",         "text/javascript"},
    {".json",       "application/json"}
};

HTTPResponse::HTTPResponse(HTTPStatus status, const std::string &srcdir, const std::string &path, bool keep_alive)
{
    init(status, srcdir, path, keep_alive);
}

HTTPResponse::~HTTPResponse()
{
    if (m_data != nullptr)
    {
        munmap(m_data, m_file_stat.st_size);
        m_data = nullptr;
    }
}

void HTTPResponse::init(HTTPStatus status, const std::string& srcdir, const std::string& path, bool keep_alive)
{
    if(m_data)
    {
        munmap(m_data, m_file_stat.st_size);
        m_data = nullptr;
    }
    m_status = status;
    m_srcdir = srcdir;
    m_path = path;
    m_keep_alive = keep_alive;
    m_file_stat = {};
}

void HTTPResponse::get_header(Buffer& buffer)
{
    if(stat((m_srcdir + m_path).data(), &m_file_stat) < 0 || S_ISDIR(m_file_stat.st_mode))
    {
        m_status = HTTPStatus::NOT_FOUND;
        m_path = "/404.html";
        if(stat((m_srcdir + m_path).data(), &m_file_stat) < 0)
        {
            abort();
        }
    }
    else if(!(m_file_stat.st_mode & S_IROTH))
    {
        m_status = HTTPStatus::FORBIDDEN;
        m_path = "/403.html";
        if(stat((m_srcdir + m_path).data(), &m_file_stat) < 0)
        {
            abort();
        }
    }
    add_status_line(buffer);
    add_headers(buffer);
    add_content(buffer);
}

void HTTPResponse::add_status_line(Buffer& buffer)
{
    std::string status_line;
    status_line += "HTTP/1.1 " + std::to_string(static_cast<int>(m_status)) + " " + m_status_map.at(m_status) + "\r\n";
    buffer.push_back(status_line);
}

void HTTPResponse::add_headers(Buffer& buffer)
{
    buffer.push_back("Connection: ");
    if(m_keep_alive)
    {
        buffer.push_back("keep-alive\r\n");
        buffer.push_back("keep-alive: max=6, timeout=120\r\n");
    }
    else
    {
        buffer.push_back("close\r\n");
    }

    buffer.push_back("Content-type: " + get_file_type() + "\r\n");
}

void HTTPResponse::add_content(Buffer& buffer)
{
    int fd = open((m_srcdir + m_path).data(), O_RDONLY);
    if(fd < 0)
    {
        error_content(buffer, "open file failed");
        close(fd);
        return;
    }

    m_data = static_cast<char*>(mmap(nullptr, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if(m_data == MAP_FAILED)
    {
        error_content(buffer, "mmap file failed");
        m_data = nullptr;
        close(fd);
        return;
    }
    close(fd);
    buffer.push_back("Content-Length: " + std::to_string(m_file_stat.st_size) + "\r\n\r\n");
}

std::string HTTPResponse::get_file_type()
{
    size_t pos = m_path.find_last_of('.');
    if(pos == std::string::npos)
    {
        return "text/plain";
    }
    std::string suffix = m_path.substr(pos);

    auto iter = m_content_type_map.find(suffix);
    if(iter == m_content_type_map.end())
    {
        return "text/plain";
    }
    return iter->second;
}

void HTTPResponse::error_content(Buffer& buffer, std::string message)
{
    std::string body;
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    body += std::to_string(static_cast<int>(m_status)) + " : " + m_status_map.at(m_status) + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>server</em></body></html>";

    buffer.push_back("Content-Length: " + std::to_string(body.size()) + "\r\n\r\n");
    buffer.push_back(body);
}

} // namespace server
