#pragma once

#include <vector>
#include <chrono>
#include <functional>
#include <list>
#include <unordered_map>



namespace server
{

class Timer
{
public:
    using Clock = std::chrono::steady_clock;
    using callback = std::function<void()>;
    using miliseconds = std::chrono::milliseconds;
    struct TimerNode
    {
        int id;
        Timer::Clock::time_point timeout;
        callback func;
        bool operator<(const TimerNode &rhs) const
        {
            return timeout < rhs.timeout;
        }
    };

    //slot_interval: the interval between two slots, in milliseconds
    Timer(size_t slot_interval = 1, size_t slot_num = 1000);
    ~Timer();

    int add_timer(callback func, int timeout);
    void del_timer(int id);
    bool reset_timer(int id, int timeout);
    void tick();

private:
    std::vector<std::list<TimerNode>> m_wheel;
    size_t m_slot_interval;
    size_t m_slot_num;
    int m_next_id;
    Clock::time_point m_start_time;
    Clock::time_point m_last_tick_time;
    size_t m_cur_slot_1;
    size_t m_cur_slot_2;
    size_t m_cur_slot_3;

    std::unordered_map<int, std::pair<int, std::list<TimerNode>::iterator>> m_timer_map;
};

} // namespace server