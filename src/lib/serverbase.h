#pragma once

#include <memory>


#include "epoller.h"
#include "buffer.h"
#include "http_request.h"
#include "http_response.h"
#include "http_connection.h"
#include "timer.h"
#include "threadpool.h"
#include "db_connectionpool.h"
#include "log.h"
#include "router.h"





namespace server
{

class ServerBase
{
public:
    ServerBase(int port, int trig_mode, int timeout, bool is_linger, int thread_num, const std::string& srcdir,
    const std::string sql_host, int sql_port, const std::string& sql_user, const std::string& sql_password, const std::string& db_name, int max_conn,
    int timer_interval, Log::Level log_level);

    virtual ~ServerBase();

    void start();

protected:
    virtual void default_handler(const HTTPRequest& req, HTTPResponse& res);

    void add_handler(HTTPMethod method, const std::string &path, Router::handler h);
    int add_timer(std::function<void()> func, int timeout);
    std::shared_ptr<DBConnectionPool> get_dbpool() { return m_dbpool; }
    const std::string& get_srcdir() { return m_srcdir; }

private:

    void accept_connection(); 
    void close_connection(int fd);
    void close_fd(int fd);
    void process_read(HTTPConnection &conn);
    void process_write(HTTPConnection& conn);
    void process(HTTPConnection& conn);

    bool is_running;// 服务器是否正在运行

    int m_port; // 端口
    int m_timeout;// 超时时间
    bool m_is_linger; // 是否开启linger
    int m_listen_fd; 
    int m_max_connection; // 最大连接数

    uint32_t m_listen_event; // 监听事件
    uint32_t m_connection_event; // 连接事件

    std::string m_srcdir; // 静态资源路径

    std::unique_ptr<Epoller> m_epoller; // epoll
    std::unique_ptr<ThreadPool> m_threadpool; // 线程池
    std::shared_ptr<DBConnectionPool> m_dbpool; // 数据库连接池
    std::unique_ptr<Router> m_router; // 路由

    std::unique_ptr<Timer> m_timer; // 定时器
    int m_timer_interval; // 定时器间隔

    std::queue<int> close_queue; // 需要关闭的连接队列
    std::mutex m_qmutex; // 队列锁

    std::unordered_map<int, HTTPConnection> m_connections; // 连接集合
};

} // namespace server




