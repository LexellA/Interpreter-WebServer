#include <iostream>
#include <thread>
#include "../src/lib/log.h"

void log_thread()
{
    auto thread_id = std::this_thread::get_id();
    auto id = std::hash<std::thread::id>{}(thread_id);
    // Test log_debug
    server::log_debug("This is a debug message {}", id);

    // Test log_info
    server::log_info("This is an info message {}", id);

    // Test log_warn
    server::log_warn("This is a warning message {}",id);

    // Test log_error
    server::log_error("This is an error message {}",id);

    // Test log_fatal
    server::log_fatal("This is a fatal message {}", id);

    // Flush the log
    server::Log::get_instance().flush();
}

int main()
{
    // Initialize the log
    server::Log::get_instance().init("log.txt");

    // Create multiple threads to log concurrently
    std::thread t1(log_thread);
    std::thread t2(log_thread);
    std::thread t3(log_thread);

    // Wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();

    return 0;
}