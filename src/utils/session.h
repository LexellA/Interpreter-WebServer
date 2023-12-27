#pragma once
#include "../interpreter/interpreter.h"

#include <atomic>

namespace utils
{

class Session
{
public:
    Session();
    ~Session();

    uint32_t get_ssid();
    void remove_ssid(uint32_t ssid);

    std::shared_ptr<interpreter::Interpreter> get_interpreter(uint32_t ssid);


private:
    //会话id
    std::atomic<uint32_t> now_ssid;

    //会话id与解释器的映射
    std::unordered_map<uint32_t, std::shared_ptr<interpreter::Interpreter>> m_interpreter_map;

};

} // namespace utils
