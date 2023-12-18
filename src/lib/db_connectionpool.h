#pragma once

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <memory>
#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>


namespace server
{

class DBConnectionPool
{
public:
    DBConnectionPool(const std::string &host, const std::string &user, 
    const std::string &password, const std::string &db_name, int port, int max_connections = 8);
    ~DBConnectionPool();

    using deleter = std::function<void(sql::Connection *)>;

    //connection will be freed automatically when out of scope
    std::unique_ptr<sql::Connection, deleter> get_connection();

    int get_conn_num() const { return m_connections.size(); }

private:
    std::string m_host;
    std::string m_user;
    std::string m_password;
    std::string m_db_name;
    int m_port;
    std::queue<sql::Connection*> m_connections;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};

} // namespace server