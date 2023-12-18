#pragma once

#include <vector>
#include <atomic>
#include <string>

namespace server
{

class Buffer
{
public:
    Buffer(size_t size);
    ~Buffer();

    size_t available() const{ return m_write_pos - m_read_pos; };
    size_t capacity() const { return m_buffer.capacity() - m_write_pos; };
    const char* data() const { return m_buffer.data() + m_read_pos; }

    void push_back(const char *buf, size_t len);
    void push_back(const std::string &str);
    void push_back(Buffer &buf);

    bool pop_front(size_t len){ 
        if(len > available())
            return false;
        m_read_pos += len;
        return true; 
    }

    void clear();

    ssize_t read(int fd, int* saved_errno);
    ssize_t write(int fd, int* saved_errno);

private:
    void expand(size_t size);
    void adjust();


    std::vector<char> m_buffer;
    std::atomic<size_t> m_read_pos;
    std::atomic<size_t> m_write_pos;
};

} // namespace server