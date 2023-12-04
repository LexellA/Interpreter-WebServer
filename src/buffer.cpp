

#include <cstring>
#include <sys/uio.h>
#include <stdexcept>

#include "buffer.h"

namespace server
{

Buffer::Buffer(size_t size)
    : m_buffer(size),
      m_read_pos(0),
      m_write_pos(0)
{}

Buffer::~Buffer()
{}

void Buffer::push_back(const char *buf, size_t len)
{
    if (len + 1 > capacity())
        expand(len + 1);
    memcpy(m_buffer.data() + m_write_pos, buf, len);
    m_write_pos += len;
    m_buffer[m_write_pos] = '\0';
}

void Buffer::push_back(const std::string &str)
{
    push_back(str.data(), str.size());
}

void Buffer::push_back(Buffer &buf)
{
    push_back(buf.data(), buf.available());
    buf.pop_front(buf.available());
}

void Buffer::clear()
{
    m_read_pos = 0;
    m_write_pos = 0;
    m_buffer[m_write_pos] = '\0';
}

ssize_t Buffer::read(int fd, int* saved_errno)
{
    char buf[65535];
    struct iovec iov[2];
    size_t len = capacity();

    iov[0].iov_base = m_buffer.data() + m_write_pos;
    iov[0].iov_len = len;
    iov[1].iov_base = buf;
    iov[1].iov_len = sizeof(buf);

    ssize_t n = readv(fd, iov, 2);
    if(n < 0)
    {
        *saved_errno = errno;
    }
    else if(n < len)
    {
        m_write_pos += n;
        m_buffer[m_write_pos] = '\0';
    }
    else
    {
        m_write_pos += len;
        push_back(buf, n - len);
    }
    return n;
}

ssize_t Buffer::write(int fd, int* saved_errno)
{
    size_t len = available();
    ssize_t n = ::write(fd, data(), len);
    if(n < 0)
    {
        *saved_errno = errno;
    }
    else
    {
        m_read_pos += n;
    }
    return n;
}

void Buffer::expand(size_t size)
{
    adjust();
    if(size > capacity())
    {
        m_buffer.resize(m_buffer.capacity() + size);
    }
}

void Buffer::adjust()
{
    if (m_read_pos == 0)
        return;
    size_t len = available();
    memmove(m_buffer.data(), m_buffer.data() + m_read_pos, len);
    m_read_pos = 0;
    m_write_pos = len;
    m_buffer[m_write_pos] = '\0';
}




} // namespace server