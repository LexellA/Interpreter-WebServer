#pragma once


#include <vector>
#include <chrono>
#include <functional>
#include <list>
#include <unordered_map>
#include <mutex>

#include "log.h"

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
        uint32_t id;
        Timer::Clock::time_point timeout;
        callback func;
    };

    // slot_interval计时器的最小精度，单位ms
    Timer(size_t slot_interval = 1, int slot_per_level = 1000, int level = 5);
    ~Timer();

    uint32_t add_timer(callback func, int timeout);
    void del_timer(uint32_t id);
    bool reset_timer(uint32_t id, int timeout);
    void tick();

private:
    struct node_position
    {
        int level;
        int slot;
        std::list<TimerNode>::iterator iter;
    };//记录定时器在时间轮中的位置

    std::vector<std::vector<std::list<TimerNode>>> m_wheel;
    uint32_t m_next_id;
    std::vector<int> m_cur_slot;
    std::mutex m_mutex;

    int m_slot_per_level;
    int m_max_level;

    size_t m_slot_interval;

    Clock::time_point m_cur_time;

    std::unordered_map<uint32_t, node_position> m_timer_map;
};

} // namespace server