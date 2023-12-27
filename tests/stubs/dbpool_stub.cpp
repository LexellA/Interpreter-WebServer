#include "../../src/lib/db_connectionpool.h"



namespace server
{

DBConnectionPool::DBConnectionPool(const std::string &host, const std::string &user, 
    const std::string &password, const std::string &db_name, int port, int max_connections)
    : m_host(host), m_user(user), m_password(password), m_db_name(db_name), m_port(port)
{
    m_connections.push(nullptr);
}

DBConnectionPool::~DBConnectionPool()
{
}

std::unique_ptr<sql::Connection, DBConnectionPool::deleter> DBConnectionPool::get_connection()
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *conn = driver->connect(m_host, m_user, m_password);
    conn->setSchema(m_db_name);
    return std::unique_ptr<sql::Connection, DBConnectionPool::deleter>(conn, [](sql::Connection *conn) {delete conn;});
}


}// namespace server