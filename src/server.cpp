#include "server.h"

namespace server
{

Server::Server(int port, int trig_mode, int timeout, bool is_linger, int thread_num,const std::string& srcdir,
    const std::string sql_host, int sql_port, const std::string& sql_user, const std::string& sql_password, const std::string& db_name, int max_conn,
    int timer_interval, Log::Level log_level)
    : ServerBase(port, trig_mode, timeout, is_linger, thread_num, srcdir, sql_host, sql_port, sql_user, sql_password, db_name, max_conn, timer_interval, log_level),
    m_login_handler(new utils::LoginHandler(get_dbpool())),
    m_chat_handler(new utils::ChatHandler())
{

    add_handler(HTTPMethod::POST, "/login", 
        std::bind(&utils::LoginHandler::handle_login, m_login_handler.get(), std::placeholders::_1, std::placeholders::_2)
    );

    add_handler(HTTPMethod::POST, "/register", 
        std::bind(&utils::LoginHandler::handle_register, m_login_handler.get(), std::placeholders::_1, std::placeholders::_2)
    );
    
    add_handler(HTTPMethod::GET, "/chat/send", 
        std::bind(&utils::ChatHandler::handle_chat, m_chat_handler.get(), std::placeholders::_1, std::placeholders::_2)   
    );
}

Server::~Server()
{
}


}