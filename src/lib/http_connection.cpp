
#include <sys/uio.h>


#include "http_connection.h"

namespace server
{

bool HTTPConnection::m_enable_et = false;

HTTPConnection::HTTPConnection(int fd, const struct sockaddr_in& addr)
    : m_fd(fd), m_addr(addr), m_read_buf(1024), m_write_buf(1024)
{
}

HTTPConnection::~HTTPConnection()
{
}


ssize_t HTTPConnection::read(int* saved_errno)
{
    // 读取数据到缓冲区
    ssize_t n = -1;
    do{
        n = m_read_buf.read(m_fd, saved_errno);
        if(n <= 0)
            break;
    } while (m_enable_et);
    return n;
}

ssize_t HTTPConnection::write(int* saved_errno)
{
    // 写入数据到socket
    ssize_t n = -1;
    do
    {
        n = writev(m_fd, m_iovec, m_iov_cnt);
        if(n <= 0)
        {
            if(m_iovec[0].iov_len + m_iovec[1].iov_len != 0)
            {
                *saved_errno = errno;
            }
            break;
        }

        if(static_cast<size_t>(n) > m_iovec[0].iov_len)
        {
            m_iovec[1].iov_base = static_cast<char*>(m_iovec[1].iov_base) + (n - m_iovec[0].iov_len);
            m_iovec[1].iov_len -= (n - m_iovec[0].iov_len);
            if(m_iovec[0].iov_len)
            {
                m_write_buf.pop_front(m_iovec[0].iov_len);
                m_iovec[0].iov_len = 0;
            }
        }
        else
        {
            m_iovec[0].iov_base = static_cast<char*>(m_iovec[0].iov_base) + n;
            m_iovec[0].iov_len -= n;
            m_write_buf.pop_front(n);
        }
        

    } while (m_enable_et || m_iovec[0].iov_len + m_iovec[1].iov_len > 1024);

    return n;
}


bool HTTPConnection::parse_request()
{
    m_request.reset();
    return m_request.parse(m_read_buf);
}

void HTTPConnection::make_response()
{
    m_response.get_response(m_write_buf);
    
    m_iovec[0].iov_base = const_cast<char*>(m_write_buf.data());
    m_iovec[0].iov_len = m_write_buf.available();

    if(m_response.has_mmap())
    {
        m_iovec[1].iov_base = const_cast<char*>(m_response.get_content());
        m_iovec[1].iov_len = m_response.get_content_length();
        m_iov_cnt = 2;
    }
    else
    {
        m_iovec[1].iov_base = nullptr;
        m_iovec[1].iov_len = 0;
        m_iov_cnt = 1;
    }

}

bool HTTPConnection::is_keep_alive() const
{
    return m_request.is_keep_alive();
}

} // namespace server
