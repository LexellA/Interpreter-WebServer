# Chatbot-Server

## Description

homework of BUPT program designing lesson 

### this project includes
- [x] a simple web server
- [x] a web client based on chatUI
- [x] a simple script interpreter

### functions

- parse http request and generate http response
- high performance, using epoll and thread pool
- database connection pool with RAII using unique_ptr
- timer implemented with hierarchical timer wheel
- log system with asynchronous log
- script interpreter, can run script file and send the result to client
- chat system, receive and send message based on the result of script


with class ServerBase, a server can be easily built

```c++
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

```

parameters are now hard coded, but it can be easily changed to read from config file, which is not implemented yet

## Dependencies

- mysqlcppconn
- nlohmann/json

## Environment

- C++ 20, compiler should support text formating(gcc 13)
- Linux


## Usage

change the parameters in main.cpp to your own, 
especially those about sql

after building the project, run the server
    
```bash
./chat_server
```

then you can use the browser to visit the server
