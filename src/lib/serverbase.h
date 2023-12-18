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
    int timer_interval);

    virtual ~ServerBase();

    void start();

protected:
    void add_handler(HTTPMethod method, const std::string &path, Router::handler h);
    std::shared_ptr<DBConnectionPool> get_dbpool() { return m_dbpool; }


private:
    virtual void default_handler(const HTTPRequest& req, HTTPResponse& res);

    void accept_connection();
    void close_connection(int fd);
    void close_fd(int fd);
    void process_read(HTTPConnection &conn);
    void process_write(HTTPConnection& conn);
    void process(HTTPConnection& conn);

    bool is_running;

    int m_port;
    int m_timeout;//ms
    bool m_is_linger;
    int m_listen_fd;
    int m_max_connection;

    uint32_t m_listen_event;
    uint32_t m_connection_event;

    std::string m_srcdir;

    std::unique_ptr<Epoller> m_epoller;
    std::unique_ptr<ThreadPool> m_threadpool;
    std::shared_ptr<DBConnectionPool> m_dbpool;
    std::unique_ptr<Router> m_router;

    std::unique_ptr<Timer> m_timer;
    int m_timer_interval;

    std::queue<int> close_queue;
    std::mutex m_qmutex;

    std::unordered_map<int, HTTPConnection> m_connections;
};

} // namespace server




