#include "../../src/utils/session.h"

namespace utils
{


Session::Session(){}

Session::~Session(){}

uint32_t Session::get_ssid()  {
    return 0;
}

void Session::remove_ssid(uint32_t ssid)  {
    return;
}

std::shared_ptr<interpreter::Interpreter> Session::get_interpreter(uint32_t ssid)  {
    return std::make_shared<interpreter::Interpreter>(std::make_shared<interpreter::Environment>());
}
    
} // namespace utils