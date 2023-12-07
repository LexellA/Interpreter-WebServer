#pragma once

#include <string>
#include <unordered_map>
#include <sys/stat.h>

#include "http_types.h"
#include "buffer.h"

namespace server
{

class HTTPResponse
{
public:
    HTTPResponse(HTTPStatus status, const std::string &srcdir, const std::string &path, bool keep_alive);
    ~HTTPResponse();

    void init(HTTPStatus status, const std::string& srcdir, const std::string& path, bool keep_alive);

    void get_header(Buffer& buffer);
    const char* get_content() const { return m_data; }
    size_t get_content_length() const { return m_file_stat.st_size; }



private:
    void add_status_line(Buffer& buffer);
    void add_headers(Buffer& buffer);
    void add_content(Buffer& buffer);
    void error_content(Buffer& buffer, std::string message);
    std::string get_file_type();


    static const std::unordered_map<HTTPStatus, std::string> m_status_map;
    static const std::unordered_map<std::string, std::string> m_content_type_map;

    HTTPStatus m_status;
    std::string m_srcdir;
    std::string m_path;
    
    bool m_keep_alive;

    char *m_data;
    struct stat m_file_stat;
};

} // namespace server