#include <iostream>
#include <thread>
#include "../src/lib/log.h"

void log_thread()
{
    auto thread_id = std::this_thread::get_id();
    auto id = std::hash<std::thread::id>{}(thread_id);
    server::log_debug("This is a debug message {}", id);

    server::log_info("This is an info message {}", id);

    server::log_warn("This is a warning message {}",id);

    server::log_error("This is an error message {}",id);

    server::log_fatal("This is a fatal message {}", id);

    server::Log::get_instance().flush();
}

int main()
{
    server::Log::get_instance().init(server::Log::Level::DEBUG, "log.txt");

    std::thread t1(log_thread);
    std::thread t2(log_thread);
    std::thread t3(log_thread);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}