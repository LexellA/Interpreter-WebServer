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
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };


    Log();
    ~Log();

    void init(const std::string& file_name, 
              uint32_t split_lines = 5000000,
              Method m = Method::ASYNC);

    template <typename... Args>
    void write(Level level, const std::string &format, Args... args);

    void flush();

    static Log& get_instance()
    {
        static Log instance;
        return instance;
    }
private:

    void async_write();

private:
    std::string m_file_name;
    std::string m_full_file_name;
    std::string m_dir;
    std::ofstream m_fout;

    uint32_t m_day;

    uint64_t m_count;
    uint32_t m_split_lines;

    Method m_method;
    std::queue<std::string> m_log_queue;
    std::mutex m_queue_mutex;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::unique_ptr<std::thread> m_thread;

    bool m_close;
};

template <typename... Args>
void Log::write(Level level, const std::string& format, Args... args)
{
    std::string str;

    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
    auto day = std::chrono::duration_cast<std::chrono::days>(now.time_since_epoch());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_t), "%Y-%m-%d %H:%M:%S") << "." << us.count() << " ";
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

    str += std::vformat(format, std::make_format_args(args...));

    if(m_method == Method::ASYNC)
    {
        std::lock_guard<std::mutex> qlock(m_queue_mutex);

        m_log_queue.push(str);
        m_cond.notify_one();
    }
    else
    {
        lock.lock();

        m_fout << str << std::endl;

        lock.unlock();
    }


}

template <typename... Args>
inline void log_debug(const std::string& format, Args... args)
{
    Log::get_instance().write(Log::Level::DEBUG, format, args...);
}

template <typename... Args>
inline void log_info(const std::string& format, Args... args)
{
    Log::get_instance().write(Log::Level::INFO, format, args...);
}

template <typename... Args>
inline void log_warn(const std::string& format, Args... args)
{
    Log::get_instance().write(Log::Level::WARN, format, args...);
}

template <typename... Args>
inline void log_error(const std::string& format, Args... args)
{
    Log::get_instance().write(Log::Level::ERROR, format, args...);
}

template <typename... Args>
inline void log_fatal(const std::string& format, Args... args)
{
    Log::get_instance().write(Log::Level::FATAL, format, args...);
}



}// namespace server