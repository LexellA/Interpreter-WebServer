
#include <sstream>
#include <ctime>
#include <iomanip>
#include <functional>

#include "log.h"

namespace server
{
void Log::flush()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_fout.flush();
}

Log::Log()
    :m_close(true), m_count(0), m_thread(nullptr),
    m_split_lines(0), m_day(0), m_method(Method::ASYNC),
    m_queue_size(0), m_level(Level::DEBUG)
{}

Log::~Log()
{
    if(!m_close)
    {
        m_close = true;
        if(m_method == Method::ASYNC){
            m_cond_full.notify_one();
            m_thread->join();
        }
        m_fout.close();
    }
}

void Log::async_write()
{
    while(!m_close)
    {
        std::string str;

        // wait for new log
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_cond_full.wait(lock, [this](){ return m_close || !m_log_queue.empty(); });
        if(m_close)
            return;
        str = std::move(m_log_queue.front());
        m_log_queue.pop();
        m_cond_empty.notify_one();
        lock.unlock();

        // write log
        std::lock_guard<std::mutex> guard(m_mutex);
        m_fout << str << std::endl;
    }
}

void Log::init(Level level,
               const std::string& file_name,
               uint32_t split_lines,
               Method m, 
               uint32_t queue_size)
{
    m_level = level;
    m_queue_size = queue_size;

    auto pos = file_name.find_last_of('/');
    m_dir = file_name.substr(0, pos);
    if(pos == std::string::npos)
    {
        m_dir = ".";
    }
    m_file_name = file_name.substr(file_name.find_last_of('/') + 1);
    m_split_lines = split_lines;
    m_method = m;
    m_close = false;

    // get current date
    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    m_day = std::chrono::duration_cast<std::chrono::days>(now.time_since_epoch()).count();
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_t), "%Y-%m-%d") << "_";
    m_full_file_name = ss.str() + m_file_name;

    // open log file
    m_fout.open(m_dir + "/" + m_full_file_name, std::ios::app);
    if(!m_fout.is_open())
    {
        throw std::runtime_error("open log file failed: " + m_dir + "/" + m_full_file_name);
    }

    // start async write
    if(m == Method::ASYNC)
    {
        m_thread.reset(new std::thread(std::bind(&Log::async_write, this)));
    }
}






}// namespace server