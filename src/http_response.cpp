
#include <sys/mman.h>
#include <fcntl.h>

#include "http_response.h"

namespace server
{

const std::unordered_map<HTTPStatus, std::string> HTTPResponse::m_status_map =
{
    {HTTPStatus::UNKNOWN, "Unknown"},
    {HTTPStatus::OK, "OK"},
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
    {"html",       "text/html"},
    {"xml",        "text/xml"},
    {"xhtml",      "application/xhtml+xml"},
    {"txt",        "text/plain"},
    {"rtf",        "application/rtf"},
    {"pdf",        "application/pdf"},
    {"word",       "application/msword"},
    {"png",        "image/png"},
    {"gif",        "image/gif"},
    {"jpg",        "image/jpeg"},
    {"jpeg",       "image/jpeg"},
    {"au",         "audio/basic"},
    {"mpeg",       "video/mpeg"},
    {"mpg",        "video/mpeg"},
    {"avi",        "video/x-msvideo"},
    {"gz",         "application/x-gzip"},
    {"tar",        "application/x-tar"},
    {"css",        "text/css"},
    {"js",         "text/javascript"},
    {"json",       "application/json"}
};

HTTPResponse::HTTPResponse()
    : m_status(HTTPStatus::UNKNOWN), m_data(nullptr), m_keep_alive(false), m_file_stat({})
{
}

HTTPResponse::~HTTPResponse()
{
    if (m_data != nullptr)
    {
        munmap(m_data, m_file_stat.st_size);
        m_data = nullptr;
    }
}

void HTTPResponse::init(HTTPStatus status, bool keep_alive, const std::string &content, const std::string &content_type)
{
    m_status = status;

    if(m_status_map.count(status) == 0)
    {
        m_status = HTTPStatus::BAD_REQUEST;
    }

    m_content = content;
    m_keep_alive = keep_alive;
    
    m_headers.clear();

    if(keep_alive)
    {
        m_headers["Connection"] = "keep-alive";
        m_headers["Keep-Alive"] = "timeout=120, max=6";
    }
    else
    {
        m_headers["Connection"] = "close";
    }

    if(m_content.size() > 0){
        m_headers["Content-type"] = get_type(content_type);
        m_headers["Content-length"] = std::to_string(m_content.size());
    }

    if(m_data != nullptr)
    {
        munmap(m_data, m_file_stat.st_size);
        m_data = nullptr;
    }
}

void HTTPResponse::set_file(const std::string &srcdir, const std::string &path)
{
    std::string filepath;
    filepath = srcdir + path;

    if (stat(filepath.c_str(), &m_file_stat) < 0 || S_ISDIR(m_file_stat.st_mode))
    {
        m_status = HTTPStatus::NOT_FOUND;
        filepath = srcdir + "/404.html";
        stat(filepath.c_str(), &m_file_stat);
    }
    else if(!(m_file_stat.st_mode & S_IROTH))
    {
        m_status = HTTPStatus::FORBIDDEN;
        filepath = srcdir + "/403.html";
        stat(filepath.c_str(), &m_file_stat);
    }

    int fd = open(filepath.c_str(), O_RDONLY);
    if(fd < 0)
    {
        m_status = HTTPStatus::NOT_FOUND;
        m_content = error_html("File Not Found!");
        m_headers["Content-type"] = "text/html";
        return;
    }

    m_data = static_cast<char *>(mmap(nullptr, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    close(fd);

    if(m_data == MAP_FAILED)
    {
        m_status = HTTPStatus::NOT_FOUND;
        m_content = error_html("File Not Found!");
        m_headers["Content-type"] = "text/html";
        return;
    }
    m_headers["Content-type"] = get_type(path.substr(path.find_last_of('.') + 1));
    m_headers["Content-length"] = std::to_string(m_file_stat.st_size);
}

void HTTPResponse::get_response(Buffer &buffer)
{
    buffer.push_back("HTTP/1.1 " + std::to_string(static_cast<int>(m_status)) + " " + m_status_map.at(m_status) + "\r\n");

    for(auto &header : m_headers)
    {
        buffer.push_back(header.first + ": " + header.second + "\r\n");
    }

    buffer.push_back("\r\n");

    if(m_content.size() > 0)
    {
        buffer.push_back(m_content);
    }
}

std::string HTTPResponse::error_html(const std::string& msg)
{
    std::string body;
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    body += std::to_string(static_cast<int>(m_status)) + " : " + m_status_map.at(m_status) + "\n";
    body += "<p>" + msg + "</p>";
    body += "<hr><em>Web Server</em></body></html>";
    return body;
}

std::string HTTPResponse::get_type(const std::string &suffix)
{
    if (m_content_type_map.count(suffix) == 0)
    {
        return m_content_type_map.at("txt");
    }
    return m_content_type_map.at(suffix);
}


} // namespace server
