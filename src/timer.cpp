


#include "timer.h"

namespace server
{

Timer::Timer(size_t slot_interval, size_t slot_num)
    : m_wheel(slot_num * 3), m_next_id(0), 
    m_cur_slot_1(0), m_cur_slot_2(0), m_cur_slot_3(0),
    m_slot_interval(slot_interval), m_slot_num(slot_num),
    m_start_time(Clock::now()), m_last_tick_time(Clock::now())
{
}

Timer::~Timer()
{
}

int Timer::add_timer(callback func, int timeout)
{
    TimerNode node;
    node.id = m_next_id++;
    node.timeout = Clock::now() + std::chrono::milliseconds(timeout);
    node.func = func;

    miliseconds pass_time = std::chrono::duration_cast<miliseconds>(node.timeout - m_start_time);
    size_t slot_3 = (pass_time.count() / (m_slot_num * m_slot_num)) % m_slot_num;
    size_t pos = slot_3 + 2 * m_slot_num;

    m_wheel[pos].push_back(node);

    m_timer_map[node.id] = std::make_pair(pos, --m_wheel[pos].end());

    return node.id;
}

void Timer::del_timer(int id)
{
    auto it = m_timer_map.find(id);
    if(it == m_timer_map.end())
        return;
    size_t pos = it->second.first;
    auto it2 = it->second.second;
    m_wheel[pos].erase(it2);
    m_timer_map.erase(it);
}

bool Timer::reset_timer(int id, int timeout)
{
    auto it = m_timer_map.find(id);
    if(it == m_timer_map.end())
        return false;
    size_t pos = it->second.first;
    auto it2 = it->second.second;
    m_wheel[pos].erase(it2);

    TimerNode node;
    node.id = id;
    node.timeout = Clock::now() + std::chrono::milliseconds(timeout);
    node.func = it2->func;
    miliseconds pass_time = std::chrono::duration_cast<miliseconds>(node.timeout - m_start_time);
    size_t slot_3 = (pass_time.count() / (m_slot_num * m_slot_num)) % m_slot_num;
    size_t pos2 = slot_3 + 2 * m_slot_num;
    m_wheel[pos2].push_back(node);
    m_timer_map[id] = std::make_pair(pos2, --m_wheel[pos2].end());
    return true;
}

void Timer::tick()
{
    Clock::time_point now = Clock::now();
    miliseconds pass_time = std::chrono::duration_cast<miliseconds>(now - m_last_tick_time);
    m_last_tick_time = now;

    for (int64_t i = 0; i < pass_time.count(); i += m_slot_interval)
    {
        m_cur_slot_1++;
        if (m_cur_slot_1 >= m_slot_num)
        {
            m_cur_slot_1 = 0;
            m_cur_slot_2++;
            if (m_cur_slot_2 >= m_slot_num)
            {
                m_cur_slot_2 = 0;
                m_cur_slot_3++;
                if (m_cur_slot_3 >= m_slot_num)
                {
                    m_cur_slot_3 = 0;
                }
            }
        }
        
        
        for(auto& node : m_wheel[m_cur_slot_3 + 2 * m_slot_num])
        {
            TimerNode node_ = node;
            miliseconds pass_time = std::chrono::duration_cast<miliseconds>(node_.timeout - m_start_time);
            size_t slot_2 = (pass_time.count() / m_slot_num) % m_slot_num;
            size_t pos = slot_2 + m_slot_num;
            m_wheel[pos].push_back(node_);
            m_timer_map[node_.id] = std::make_pair(pos, --m_wheel[pos].end());
        }
        m_wheel[m_cur_slot_3 + 2 * m_slot_num].clear();

        for(auto &node : m_wheel[m_cur_slot_2 + m_slot_num])
        {
            TimerNode node_ = node;
            miliseconds pass_time = std::chrono::duration_cast<miliseconds>(node_.timeout - m_start_time);
            size_t slot_1 = pass_time.count() % m_slot_num;
            size_t pos = slot_1;
            m_wheel[pos].push_back(node_);
            m_timer_map[node_.id] = std::make_pair(pos, --m_wheel[pos].end());
        }
        m_wheel[m_cur_slot_2 + m_slot_num].clear();

        for(auto &node : m_wheel[m_cur_slot_1])
        {
            node.func();
        }
        m_wheel[m_cur_slot_1].clear();
    }
}

}// namespace server