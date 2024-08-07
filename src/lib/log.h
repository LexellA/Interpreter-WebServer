#pragma once


#include <fstream>
#include <string>
#include <chrono>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#include <format>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <utility>



namespace server
{

class Log
{
public:
    enum class Method
    {
        ASYNC,
        SYNC
    };

    enum class Level
    {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG
    };


    Log();
    ~Log();

    // 初始化日志，level为日志级别，file_name为日志文件名，split_lines为日志文件最大行数
    // m为日志写入方式，queue_size为异步队列大小
    void init(Level level, 
              const std::string& file_name, 
              uint32_t split_lines = 500000,
              Method m = Method::ASYNC,
              uint32_t queue_size = 2048);

    template <typename... Args>
    void write(Level level, std::format_string<Args...> format,
                    Args&&... args);

    void flush();

    static Log& get_instance()
    {
        static Log instance;
        return instance;
    }
private:

    void async_write();

private:
    Level m_level;

    std::string m_file_name;
    std::string m_full_file_name;
    std::string m_dir;
    std::ofstream m_fout;

    uint32_t m_day;

    uint64_t m_count;
    uint32_t m_split_lines;

    Method m_method;
    std::queue<std::string> m_log_queue;
    uint32_t m_queue_size;
    std::mutex m_queue_mutex;
    std::mutex m_mutex;
    std::condition_variable m_cond_empty;
    std::condition_variable m_cond_full;
    std::unique_ptr<std::thread> m_thread;

    bool m_close;
};

template <typename... Args>
void Log::write(Level level, std::format_string<Args...> format, Args&&... args)
{
    if(level > m_level)
        return;

    std::string str;

    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
    auto day = std::chrono::duration_cast<std::chrono::days>(now.time_since_epoch());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_t), "%Y-%m-%d %H:%M:%S") << "." << std::setw(6) << std::setfill('0') << us.count() << " ";
    str += ss.str();

    switch(level)
    {
        case Level::DEBUG:
            str += "[DEBUG]: ";
            break;
        case Level::INFO:
            str += "[INFO]: ";
            break;
        case Level::WARN:
            str += "[WARN]: ";
            break;
        case Level::ERROR:
            str += "[ERROR]: ";
            break;
        case Level::FATAL:
            str += "[FATAL]: ";
            break;
        default:
            throw std::runtime_error("unknown log level");
            break;
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    m_count++;

    // 每天创建一个日志文件，或者日志文件行数达到最大值时创建新的日志文件
    if (m_day != day.count())
    {
        m_day = day.count();
        m_fout.close();
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_t), "%Y-%m-%d") << "_";
        m_full_file_name = time_ss.str() + m_file_name;
        m_count = 1;
        m_fout.open(m_dir + "/" + m_full_file_name, std::ios::app);
    }
    else if(m_count >= m_split_lines)
    {
        m_fout.close();
        std::stringstream time_ss;
        time_ss << std::put_time(std::localtime(&now_t), "%Y-%m-%d") << "_";
        m_full_file_name = time_ss.str() + m_file_name + "_" + std::to_string(m_count / m_split_lines);
        m_fout.open(m_dir + "/" + m_full_file_name, std::ios::app);
    }
    if(!m_fout.is_open())
    {
        throw std::runtime_error("open log file failed: " + m_dir + "/" + m_full_file_name);
    }

    lock.unlock();

    str += std::format(format, std::forward<Args>(args)...);

    if(m_method == Method::ASYNC)
    {
        // 异步写入
        std::unique_lock<std::mutex> qlock(m_queue_mutex);
        m_cond_empty.wait(qlock, [this](){ return m_log_queue.size() < m_queue_size; });

        m_log_queue.push(str);

        m_cond_full.notify_one();
    }
    else
    {
        // 同步写入
        lock.lock();

        m_fout << str << std::endl;

        lock.unlock();
    }
}

//usage: log_debug("hello world, {}", "format");
template <typename... Args>
inline void log_debug(std::format_string<Args...> format, Args&&... args) {
    Log::get_instance().write(Log::Level::DEBUG, format, std::forward<Args>(args)...);
}

//usage: log_info("hello world, {}", "format");
template <typename... Args>
inline void log_info(std::format_string<Args...> format, Args&&... args)
{
    Log::get_instance().write(Log::Level::INFO, format, std::forward<Args>(args)...);
}

//usage: log_warn("hello world, {}", "format");
template <typename... Args>
inline void log_warn(std::format_string<Args...> format, Args&&... args)
{
    Log::get_instance().write(Log::Level::WARN, format, std::forward<Args>(args)...);
}

//usage: log_error("hello world, {}", "format");
template <typename... Args>
inline void log_error(std::format_string<Args...> format, Args&&... args)
{
    Log::get_instance().write(Log::Level::ERROR, format, std::forward<Args>(args)...);
}

//usage: log_fatal("hello world, {}", "format");
template <typename... Args>
inline void log_fatal(std::format_string<Args...> format, Args&&... args)
{
    Log::get_instance().write(Log::Level::FATAL, format, std::forward<Args>(args)...);
}



}// namespace server