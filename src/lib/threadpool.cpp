


#include "threadpool.h"

namespace server
{

ThreadPool::ThreadPool(size_t thread_num)
{
    m_stop = false;
    for(size_t i = 0; i < thread_num; ++i)
    {
        // 创建线程并设置线程函数
        m_threads.emplace_back(
            // 使用lambda表达式创建线程函数
            [this]()
            {
                // 停止前一直获取任务并执行
                while(!m_stop)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_cond.wait(lock, [this](){ return m_stop || !m_tasks.empty(); });
                        if(m_stop)
                            break;
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_cond.notify_all();
    for(auto &t : m_threads)
    {
        t.join();
    }
}

} // namespace server