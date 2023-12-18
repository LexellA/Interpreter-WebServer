
#include <unistd.h>
#include <sys/file.h>

#include "epoller.h"

namespace server
{

Epoller::Epoller(uint32_t max_events)
    : m_epoll_fd(epoll_create1(EPOLL_CLOEXEC)),
    m_events(max_events)
{
}

Epoller::~Epoller()
{
    close(m_epoll_fd);
}

bool Epoller::add(int fd, uint32_t events)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    return epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == 0;
}

bool Epoller::modify(int fd, uint32_t events)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    return epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &event) == 0;
}

bool Epoller::remove(int fd)
{
    return epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == 0;
}

int Epoller::wait(int timeout_ms)
{
    return epoll_wait(m_epoll_fd, m_events.data(), static_cast<int>(m_events.size()), timeout_ms);
}





} // namespace server
