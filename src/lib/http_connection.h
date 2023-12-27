#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "buffer.h"
#include "http_request.h"
#include "http_response.h"



namespace server
{

class HTTPConnection
{
public:
    HTTPConnection(int fd, const struct sockaddr_in& addr);
    ~HTTPConnection();

    void set_timer(uint32_t id) { timer_id = id; }
    uint32_t get_timer_id() const { return timer_id; }

    ssize_t read(int* saved_errno);
    ssize_t write(int* saved_errno);

    int get_fd() const { return m_fd; }
    const struct sockaddr_in& get_addr() const { return m_addr; }
    int get_port() const { return ntohs(m_addr.sin_port); }
    const char* get_ip() const { return inet_ntoa(m_addr.sin_addr); }
    bool is_keep_alive() const;
    int to_read() const { return m_read_buf.available(); }
    int to_write() const { return m_iovec[0].iov_len + m_iovec[1].iov_len; }

    HTTPRequest& get_request() { return m_request; }
    HTTPResponse& get_response() { return m_response; }

    bool parse_request();
    void make_response();

    static void enable_et() { m_enable_et = true; }
    static void disable_et() { m_enable_et = false; }
private:
    uint32_t timer_id;

    int m_fd;
    struct sockaddr_in m_addr;

    int m_iov_cnt;
    struct iovec m_iovec[2]; // 有文件映射的时候，用于分散读

    Buffer m_read_buf;
    Buffer m_write_buf;

    HTTPRequest m_request;
    HTTPResponse m_response;

    static bool m_enable_et; // 是否开启ET模式
};

} // namespace server