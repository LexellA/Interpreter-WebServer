#pragma once

#include <sys/epoll.h>
#include <vector>


namespace server
{

class Epoller
{
public:
    Epoller(uint32_t max_events = 1024);
    ~Epoller();

    bool add(int fd, uint32_t events);
    bool modify(int fd, uint32_t events);
    bool remove(int fd);
    int wait(int timeout_ms = -1);

    const epoll_event& get_event(int i) { return m_events[i]; }

private:
    int m_epoll_fd;
    std::vector<struct epoll_event> m_events;
};

} // namespace server