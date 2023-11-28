
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace server
{

class ThreadPool
{
public:
    ThreadPool(size_t thread_num = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 8);
    ~ThreadPool();

    template <typename Func, typename... Args>
    auto add_task(Func &&func, Args &&...args)
        -> std::future<typename std::invoke_result<Func, Args...>::type>;

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_stop;
};

template <typename Func, typename... Args>
auto ThreadPool::add_task(Func&& func, Args&& ...args)
    -> std::future<typename std::invoke_result<Func, Args...>::type>
{
    using return_type = typename std::invoke_result<Func, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );//将函数和参数打包，形成一个任务，类型为返回值为return_type，参数为void的packaged_task
    
    std::future<return_type> ret = task->get_future();
    //获取任务的future，用于获取任务的返回值

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.emplace([task](){ (*task)(); });
    }//将任务放入任务队列，使用lambda表达式，避免拷贝task

    m_cond.notify_one();

    return ret;
}

} // namespace server
