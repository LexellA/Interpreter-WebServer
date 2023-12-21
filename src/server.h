#pragma once

#include "lib/serverbase.h"
#include "lib/log.h"

#include "utils/login_handler.h"
#include "utils/chat_handler.h"

namespace server
{

class Server : public ServerBase
{

public:
    Server(int port, int trig_mode, int timeout, bool is_linger, int thread_num, const std::string& srcdir,
    const std::string sql_host, int sql_port, const std::string& sql_user, const std::string& sql_password, const std::string& db_name, int max_conn,
    int timer_interval, Log::Level log_level);

    virtual ~Server();



private:
    std::unique_ptr<utils::LoginHandler> m_login_handler;
    std::unique_ptr<utils::ChatHandler> m_chat_handler;

};

} // namespace server