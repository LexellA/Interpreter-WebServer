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
    // 端口，触发模式，超时时间，是否开启linger，线程池线程数，静态资源路径，数据库地址，数据库端口，
    // 数据库用户名，数据库密码，数据库名，最大连接数，定时器间隔，日志等级，脚本路径
    Server(int port, int trig_mode, int timeout, bool is_linger, int thread_num, const std::string& srcdir,
    const std::string sql_host, int sql_port, const std::string& sql_user, const std::string& sql_password, const std::string& db_name, int max_conn,
    int timer_interval, Log::Level log_level, const std::string& scripts_src);

    virtual ~Server();


private:
    void default_handler(const HTTPRequest& req, HTTPResponse& res) override;


    std::shared_ptr<utils::Session> m_session;
    std::unique_ptr<utils::LoginHandler> m_login_handler;
    std::unique_ptr<utils::ChatHandler> m_chat_handler;


};

} // namespace server