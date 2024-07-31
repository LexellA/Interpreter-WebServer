#include <sys/socket.h>
#include <cmath>
#include <stdexcept>
#include "log.h"
#include <fcntl.h>
#include <sys/resource.h>

#include "serverbase.h"



namespace server
{

ServerBase::ServerBase(int port, int trig_mode, int timeout, bool is_linger, int thread_num, const std::string& srcdir,
    const std::string sql_host, int sql_port, const std::string& sql_user, const std::string& sql_password, const std::string& db_name, int max_conn,
    int timer_interval, Log::Level log_level)
    : m_port(port), 
    m_timeout(timeout), 
    m_is_linger(is_linger), 
    m_listen_fd(-1), 
    m_srcdir(srcdir), 
    m_epoller(new Epoller()), 
    m_threadpool(new ThreadPool(thread_num)), 
    m_dbpool(new DBConnectionPool(sql_host, sql_user, sql_password, db_name, sql_port)), 
    m_router(new Router()),
    is_running(false),
    m_max_connection(max_conn),
    m_timer_interval(timer_interval),
    m_timer(new Timer(timer_interval))
{
    //init log
    Log::get_instance().init(log_level, "./log");

    //init triger mode
    m_listen_event = EPOLLRDHUP;
    m_connection_event = EPOLLONESHOT | EPOLLRDHUP;
    switch (trig_mode)
    {
    case 0:
        break;
    case 1:
        m_listen_event |= EPOLLET;
        break;
    case 2:
        m_connection_event |= EPOLLET;
        break;
    case 3:
        m_listen_event |= EPOLLET;
        m_connection_event |= EPOLLET;
        break;
    default:
        m_listen_event |= EPOLLET;
        m_connection_event |= EPOLLET;
        break;
    }
    if(m_connection_event & EPOLLET)
    {
        HTTPConnection::enable_et();
    }

    //init socket
    struct sockaddr_in addr{};
    if(port > 65535 || port < 1024)
    {
        log_error("Port: %d is invalid!", port);
        throw std::invalid_argument("Port is invalid!");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    struct linger opt_linger{};
    if(m_is_linger)
    {
        opt_linger.l_onoff = 1;
        opt_linger.l_linger = 1;
    }

    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_listen_fd < 0)
    {
        log_error("Create socket error!");
        throw std::runtime_error("Create socket error!");
    }
    int ret = setsockopt(m_listen_fd, SOL_SOCKET, SO_LINGER, &opt_linger, sizeof(opt_linger));
    if(ret < 0)
    {
        log_error("Set socket SO_LINGER error!");
        throw std::runtime_error("Set socket SO_LINGER error!");
    }

    int optval = 1;
    ret = setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    //地址复用
    if(ret < 0)
    {
        log_error("Set socket SO_REUSEADDR error!");
        throw std::runtime_error("Set socket SO_REUSEADDR error!");
    }

    ret = bind(m_listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        log_error("Bind socket error!");
        throw std::runtime_error("Bind socket error!");
    }

    ret = listen(m_listen_fd, 6);
    if(ret < 0)
    {
        log_error("Listen socket error!");
        throw std::runtime_error("Listen socket error!");
    }

    if(!m_epoller->add(m_listen_fd, m_listen_event | EPOLLIN))
    {
        log_error("Add listen fd to epoll error!");
        throw std::runtime_error("Add listen fd to epoll error!");
    }

    if(fcntl(m_listen_fd, F_SETFL, fcntl(m_listen_fd, F_GETFD, 0) | O_NONBLOCK) < 0)
    {
        log_error("Set listen fd nonblock error!");
        throw std::runtime_error("Set listen fd nonblock error!");
    }
    
    //init rlimit
    struct rlimit rt{};
    rt.rlim_max = rt.rlim_cur = m_max_connection + 512;
    if(setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        log_error("Setrlimit error!");
        throw std::runtime_error("Setrlimit error!");
    }


    //init router
    m_router->add_handler(HTTPMethod::GET, "default", std::bind(&ServerBase::default_handler, this, std::placeholders::_1, std::placeholders::_2));

    log_info("Server init success!");
    log_info("Listen port: {}", port);
    log_info("Triger mode: {}", trig_mode);
    log_info("Timeout: {} ms", timeout);
    log_info("Linger: {}", is_linger ? "true" : "false");
    log_info("Thread number: {}", thread_num);
    log_info("Source directory: {}", srcdir);
    log_info("SQL host: {}", sql_host);
    log_info("SQL port: {}", sql_port);
    log_info("SQL user: {}", sql_user);
    log_info("threadpool max thread number: {}", thread_num);

    is_running = true;

}

ServerBase::~ServerBase()
{
    close(m_listen_fd);
}

void ServerBase::start()
{
    // 输入exit退出服务器
    std::thread input_thread([this]()
    {
        std::string cmd;
        while(true)
        {
            std::cin >> cmd;
            if(cmd == "exit")
            {
                is_running = false;
                break;
            }
        }
    });

    while(is_running)
    {
        m_timer->tick();

        std::unique_lock<std::mutex> lock(m_qmutex);
        while(!close_queue.empty())
        {
            int fd = close_queue.front();
            close_connection(fd);
            close_queue.pop();
        }
        lock.unlock();


        int event_num = m_epoller->wait(m_timer_interval);
        bool new_conn = false;
        log_debug("start event dispatch");
        for(int i = 0; i < event_num; i++)
        {
            int fd = m_epoller->get_event(i).data.fd;
            uint32_t events = m_epoller->get_event(i).events;
            if(fd == m_listen_fd)
            {
                // 新连接
                new_conn = true;
            }
            else if(events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                // 关闭连接
                close_connection(fd);
            }
            else if(events & EPOLLOUT)
            {
                // 可写事件
                auto it = m_connections.find(fd);
                if(it == m_connections.end())
                {
                    log_error("Invalid connection fd: {}", fd);
                    continue;
                }
                log_info("Write event, fd: {}", fd);

                m_timer->reset_timer(it->second.get_timer_id(), m_timeout);
                m_threadpool->add_task(&ServerBase::process_write, this, std::ref(it->second));
            }
            else if(events & EPOLLIN)
            {
                // 可读事件
                auto it = m_connections.find(fd);
                if(it == m_connections.end())
                {
                    log_error("Invalid connection fd: {}", fd);
                    continue;
                }
                log_info("Read event, fd: {}", fd);

                m_timer->reset_timer(it->second.get_timer_id(), m_timeout);
                m_threadpool->add_task(&ServerBase::process_read, this, std::ref(it->second));
            }
        }
        log_debug("end event dispatch");

        if(new_conn)
        {
            accept_connection();
        }
    }

    input_thread.join();
}

void ServerBase::accept_connection()
{
    struct sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);
    do
    {

        int conn_fd = accept(m_listen_fd, (struct sockaddr*)&addr, &addr_len);
        uint32_t timer_id;

        if(conn_fd <= 0)
        {
            switch(errno)
            {
                case EAGAIN:
                    return; // 没有新连接了
                case EBADF:
                    log_error("Invalid listening socket!");
                    return; 
                case ECONNABORTED:
                    continue; // 被中断，继续循环
                case EMFILE:
                case ENFILE:
                    log_error("File descriptor limit reached!");
                    return; 
                default:
                    log_error("Unknown error in accept!");
                    return;
            }
        }
        // 达到最大连接数
        if(m_connections.size() >= m_max_connection)
        {
            log_warn("Too many connections!");
            send(conn_fd, "Server busy!", 13, 0);
            close(conn_fd);
            continue;
        }

        // 添加新连接
        auto res = m_connections.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(conn_fd),
            std::forward_as_tuple(conn_fd, addr)
        );

        // 添加失败
        if(!res.second)
        {
            log_error("Add connection error!");
            close_fd(conn_fd);
            continue;
        }

        // 添加到epoll
        if(!m_epoller->add(conn_fd, m_connection_event | EPOLLIN))
        {
            log_error("Add connection fd to epoll error!");
            close_fd(conn_fd);
            continue;
        }

        // 设置非阻塞
        if(fcntl(conn_fd, F_SETFL, fcntl(conn_fd, F_GETFD, 0) | O_NONBLOCK) < 0)
        {
            log_error("Set connection fd nonblock error!");
            close_fd(conn_fd);
            continue;
        }

        // 设置超时
        if(m_timeout > 0)
        {
            timer_id = m_timer->add_timer(std::bind(&ServerBase::close_fd, this, conn_fd), m_timeout);
            res.first->second.set_timer(timer_id);
        }

        log_info("New connection from {}:{} fd: {}", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), conn_fd);

    } while (m_listen_event & EPOLLET);
}

void ServerBase::close_fd(int fd)
{
    std::unique_lock<std::mutex> lock(m_qmutex);
    close_queue.push(fd);
}

void ServerBase::close_connection(int fd)
{
    auto it = m_connections.find(fd);
    if(it == m_connections.end())
    {
        log_error("closing invalid connection, fd: {}", fd);
        return;
    }
    m_timer->del_timer(it->second.get_timer_id());
    close(fd);
    m_connections.erase(fd);
    m_epoller->remove(fd);
    log_info("Close connection, fd: {}", fd);
}

void ServerBase::process_read(HTTPConnection& conn)
{
    int ret = -1;
    int saved_errno = 0;
    ret = conn.read(&saved_errno);
    if(ret <= 0 && saved_errno != EAGAIN)
    {
        close_fd(conn.get_fd());
        return;
    }

    process(conn);
}

void ServerBase::process_write(HTTPConnection& conn)
{
    int ret = -1;
    int saved_errno = 0;
    ret = conn.write(&saved_errno);

    if(conn.to_write() == 0 && conn.is_keep_alive())
    {
        process(conn);
        return;
    }
    
    if(ret < 0 && saved_errno == EAGAIN)
    {
        m_epoller->modify(conn.get_fd(), m_connection_event | EPOLLOUT);
        return;
    }

    close_fd(conn.get_fd());
}

void ServerBase::process(HTTPConnection& conn)
{
    if(conn.to_read() == 0)
    {
        m_epoller->modify(conn.get_fd(), m_connection_event | EPOLLIN);
        return;
    }

    bool parse_ok = conn.parse_request();
    
    if(!parse_ok)
    {
        log_error("Parse request error!");
    }
    else
    {
        log_debug(
            "Request from {}:{} fd: {} method: {} path: {} version: {}",
            conn.get_ip(), conn.get_port(), conn.get_fd(), conn.get_request().get_method_str(), 
            conn.get_request().get_path(), conn.get_request().get_version_str()
        );
    }

    try
    {
        m_router->route(conn.get_request(), conn.get_response());
    }
    catch(const std::exception& e)
    {
        log_error("{}", e.what());
        conn.get_response().init(HTTPStatus::BAD_REQUEST, false, "Bad Request", "txt");
    }

    log_debug(
        "Response to {}:{} fd: {} status: {} content_length: {}",
        conn.get_ip(), conn.get_port(), conn.get_fd(), 
        conn.get_response().get_status_str(),
        conn.get_response().get_content_length()
    );

    conn.make_response();

    m_epoller->modify(conn.get_fd(), m_connection_event | EPOLLOUT);
}

void ServerBase::default_handler(const HTTPRequest& req, HTTPResponse& res)
{
    res.init(HTTPStatus::OK, req.is_keep_alive());
    res.set_file(m_srcdir, req.get_path());
}

void ServerBase::add_handler(HTTPMethod method, const std::string &path, Router::handler h)
{
    m_router->add_handler(method, path, h);
}

int ServerBase::add_timer(std::function<void()> func, int timeout)
{
    return m_timer->add_timer(func, timeout);
}


}// namespace server