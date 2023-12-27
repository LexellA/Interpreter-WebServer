#include "../../src/interpreter/environment.h"

namespace interpreter
{
Environment::Environment() {}

Environment::~Environment() {}

void Environment::add_variable(const std::string &name, double value)
{
    return;
}

void Environment::add_variable(const std::string &name, const std::string &value)
{
    return;
}

void Environment::add_function(const std::string &name, std::shared_ptr<Func> func)
{
    return;
}

void Environment::set_variable(const std::string &name, double value)
{
    return;
}

void Environment::set_variable(const std::string &name, const std::string &value)
{
    return;
}

void Environment::get_vartype(const std::string &name, ValueType &type)
{
    type = ValueType::NUMBER;
    return;
}

void Environment::get_variable(const std::string &name, double &value)
{
    value = 0;
    return;
}

void Environment::get_variable(const std::string &name, std::string &value)
{
    value = "";
    return;
}

std::shared_ptr<interpreter::Func> Environment::get_function(const std::string &name)
{
    return nullptr;
}

void Environment::set_send_msg(const std::string &msg)
{
    return;
}

void Environment::set_recv_msg(const std::string &msg)
{
    return;
}

const std::string &Environment::get_send_msg() const
{
    static std::string msg1 = "send_msg";
    return msg1;
}

const std::string &Environment::get_recv_msg() const
{
    static std::string msg2 = "recv_msg";
    return msg2;
}

void Environment::push(std::shared_ptr<ASTNode> node, bool state)
{
    return;
}

std::pair<std::shared_ptr<ASTNode>, bool> Environment::pop()
{
    return std::make_pair(nullptr, false);
}

bool Environment::empty()
{
    static int times = 5;
    if (times > 0)
    {
        times--;
        return true;
    }
    return false;
}


} // namespace interpreter