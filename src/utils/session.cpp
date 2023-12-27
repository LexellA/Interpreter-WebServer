
#include "session.h"
#include <iostream>

namespace utils
{

Session::Session()
{
    now_ssid = 0;
}

Session::~Session()
{
}

uint32_t Session::get_ssid()
{
    m_interpreter_map[now_ssid] = std::make_shared<interpreter::Interpreter>(std::make_shared<interpreter::Environment>());
    return now_ssid++;
}

void Session::remove_ssid(uint32_t ssid)
{
    m_interpreter_map[ssid] = nullptr;
}

std::shared_ptr<interpreter::Interpreter> Session::get_interpreter(uint32_t ssid)
{
    return m_interpreter_map[ssid];
}



}