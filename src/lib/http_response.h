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
    HTTPResponse();
    ~HTTPResponse();

    // 初始化， 传入状态码，是否保持连接，内容，内容类型
    void init(HTTPStatus status, bool keep_alive, const std::string &content = "", const std::string &content_type = "");
    
    // 设置文件，传入静态资源路径和文件路径
    void set_file(const std::string &srcdir, const std::string &path);

    // 添加header
    void add_header(const std::string &key, const std::string &value){
        m_headers[key] = value;
    }

    // 获取响应
    void get_response(Buffer &buffer);
    // 是否进行文件映射
    bool has_mmap() const { return m_data != nullptr; }
    const char *get_content() const { return m_data; }
    size_t get_content_length() const 
    {
        return m_headers.count("Content-length") ? std::stoi(m_headers.at("Content-length")) : 0;
    }
    
    const std::string& get_status_str() const { return m_status_map.at(m_status); }

private:
    std::string get_type(const std::string &suffix);
    std::string error_html(const std::string& msg);

    static const std::unordered_map<HTTPStatus, std::string> m_status_map;
    static const std::unordered_map<std::string, std::string> m_content_type_map;

    HTTPStatus m_status;
    std::string m_srcdir;
    std::string m_path;

    std::unordered_map<std::string, std::string> m_headers;
    std::string m_content;
    
    bool m_keep_alive;

    char *m_data;
    struct stat m_file_stat;
};

} // namespace server