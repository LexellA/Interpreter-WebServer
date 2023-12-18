


#include "db_connectionpool.h"

namespace server
{

DBConnectionPool::DBConnectionPool(const std::string &host, const std::string &user, 
    const std::string &password, const std::string &db_name, int port, int max_connections)
    : m_host(host), m_user(user), m_password(password), m_db_name(db_name), m_port(port)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    for(int i = 0; i < max_connections; ++i)
    {
        sql::Connection *conn = driver->connect(m_host, m_user, m_password);
        conn->setSchema(m_db_name);
        m_connections.push(conn);
    }
}

DBConnectionPool::~DBConnectionPool()
{
    while(!m_connections.empty())
    {
        sql::Connection *conn = m_connections.front();
        m_connections.pop();
        delete conn;
    }
}

std::unique_ptr<sql::Connection, DBConnectionPool::deleter> 
DBConnectionPool::get_connection()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [this](){ return !m_connections.empty(); });
    sql::Connection *conn = m_connections.front();
    m_connections.pop();
    
    return std::unique_ptr<sql::Connection, deleter>(conn, [this](sql::Connection *conn)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push(conn);
        m_cond.notify_one();
    });
}

} // namespace server