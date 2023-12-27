

#include "timer.h"

namespace server
{

Timer::Timer(size_t slot_interval, int slot_per_level, int level)
    : m_wheel(level, std::vector<std::list<TimerNode>>(slot_per_level)),
    m_next_id(0),
    m_slot_per_level(slot_per_level),
    m_max_level(level),
    m_cur_slot(level, 0),
    m_slot_interval(slot_interval),
    m_cur_time(Clock::now())
{
}

Timer::~Timer()
{
}

uint32_t Timer::add_timer(callback func, int timeout)
{
    // 创建定时器节点
    TimerNode node;
    node.id = m_next_id++;
    node.timeout = m_cur_time + miliseconds(timeout);
    node.func = func;

    // 计算定时器在时间轮中的位置
    int level = 0;
    int ticks = timeout / m_slot_interval;
    while(ticks >= m_slot_per_level)
    {
        ticks = (ticks + m_cur_slot[level]) / m_slot_per_level;
        level++;
    }
    int slot = (m_cur_slot[level] + ticks) % m_slot_per_level;

    m_wheel[level][slot].push_back(node);

    // 记录定时器在时间轮中的位置
    node_position pos;
    pos.level = level;
    pos.slot = slot;
    pos.iter = std::prev(m_wheel[level][slot].end());
    m_timer_map[node.id] = pos;

    log_debug("add timer {} at level {} slot {}", node.id, level, slot);

    return node.id;
}

void Timer::del_timer(uint32_t id)
{
    // 通过id找到定时器在时间轮中的位置，然后删除
    auto it = m_timer_map.find(id);
    if(it == m_timer_map.end())
    {
        return;
    }
    log_debug("del timer {} at level {} slot {}", it->second.iter->id, it->second.level, it->second.slot);
    m_wheel[it->second.level][it->second.slot].erase(it->second.iter);
}

bool Timer::reset_timer(uint32_t id, int timeout)
{
    // 通过id找到定时器在时间轮中的位置，然后重新设置
    auto it = m_timer_map.find(id);
    if(it == m_timer_map.end())
    {
        return false;
    }

    auto &node = *it->second.iter;
    node.timeout = m_cur_time + miliseconds(timeout);

    int level = 0;
    int ticks = timeout / m_slot_interval;
    while(ticks >= m_slot_per_level)
    {
        ticks = (ticks + m_cur_slot[level]) / m_slot_per_level;
        level++;
    }
    int slot = (m_cur_slot[level] + ticks) % m_slot_per_level;
    
    m_wheel[level][slot].splice(m_wheel[level][slot].end(), m_wheel[it->second.level][it->second.slot], it->second.iter);
    it->second.level = level;
    it->second.slot = slot;
    it->second.iter = std::prev(m_wheel[level][slot].end());

    log_debug("reset timer {} at level {} slot {}", node.id, level, slot);

    return true;
}

void Timer::tick()
{
    // 计算时间轮转动的格数
    Clock::time_point now = Clock::now();
    int ticks = std::chrono::duration_cast<miliseconds>(now - m_cur_time).count() / m_slot_interval;
    m_cur_time = now;

    while(ticks--)
    {
        // 时间轮转动一格
        m_cur_slot[0] = (m_cur_slot[0] + 1) % m_slot_per_level;

        int level = 0;
        while (level < m_max_level - 1 && m_cur_slot[level] == 0)
        {
            level++;
            m_cur_slot[level] = (m_cur_slot[level] + 1) % m_slot_per_level;

            int slot = m_cur_slot[level];

            for (auto it = m_wheel[level][slot].begin(); it != m_wheel[level][slot].end();)
            {
                auto re_time = std::chrono::duration_cast<miliseconds>(it->timeout - m_cur_time).count();
                int re_ticks = re_time / m_slot_interval;
                if (re_time <= 0)
                {
                    log_debug("timer {} timeout", it->id);
                    it->func();
                    it = m_wheel[level][slot].erase(it);
                    m_timer_map.erase(it->id);
                }
                else
                {
                    int next_level = 0;
                    while (re_ticks >= m_slot_per_level)
                    {
                        re_ticks = (re_ticks + m_cur_slot[next_level]) / m_slot_per_level;
                        next_level++;
                    }
                    int next_slot = (m_cur_slot[next_level] + re_ticks) % m_slot_per_level;

                    auto tmp = it;
                    m_wheel[next_level][next_slot].splice(m_wheel[next_level][next_slot].end(), m_wheel[level][slot], it);
                    it = std::next(tmp);
                }
            }
        }

        for(auto &node : m_wheel[0][m_cur_slot[0]])
        {
            log_debug("timer {} timeout", node.id);
            node.func();
            m_timer_map.erase(node.id);
        }
        m_wheel[0][m_cur_slot[0]].clear();
    }
}

} // namespace server