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

    ssize_t read(int* saved_errno);
    ssize_t write(int* saved_errno);

    int get_fd() const { return m_fd; }
    const struct sockaddr_in& get_addr() const { return m_addr; }
    int get_port() const { return ntohs(m_addr.sin_port); }
    const char* get_ip() const { return inet_ntoa(m_addr.sin_addr); }

    HTTPRequest& get_request() { return m_request; }
    HTTPResponse& get_response() { return m_response; }

    void parse_request();
    void make_response();

    static void enable_et() { m_enable_et = true; }
    static void disable_et() { m_enable_et = false; }
private:

    int m_fd;
    struct sockaddr_in m_addr;

    int m_iov_cnt;
    struct iovec m_iovec[2];

    Buffer m_read_buf;
    Buffer m_write_buf;

    HTTPRequest m_request;
    HTTPResponse m_response;

    static bool m_enable_et;
};

} // namespace server