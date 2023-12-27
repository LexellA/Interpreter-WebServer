#include "environment.h"

namespace interpreter
{

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::add_variable(const std::string &name, double value)
{
    m_types[name] = ValueType::NUMBER;
    m_variables[name] = value;
}

void Environment::add_variable(const std::string &name, const std::string &value)
{
    m_types[name] = ValueType::STRING;
    m_variables[name] = value;
}

void Environment::add_function(const std::string &name, std::shared_ptr<Func> func)
{
    m_functions[name] = func;
}

void Environment::set_variable(const std::string &name, double value)
{
    m_variables[name] = value;
    m_types[name] = ValueType::NUMBER;
}

void Environment::set_variable(const std::string &name, const std::string &value)
{
    m_variables[name] = value;
    m_types[name] = ValueType::STRING;

}

void Environment::get_vartype(const std::string &name, ValueType &type)
{
    type = m_types.at(name);
}

void Environment::get_variable(const std::string &name, double &value)
{
    value = std::get<double>(m_variables.at(name));
}

void Environment::get_variable(const std::string &name, std::string &value)
{
    value = std::get<std::string>(m_variables.at(name));
}

std::shared_ptr<Func> Environment::get_function(const std::string &name)
{
    return m_functions.at(name);
}

void Environment::push(std::shared_ptr<ASTNode> node, bool state)
{
    m_stack.push(node);
    m_state_stack.push(state);
}

std::pair<std::shared_ptr<ASTNode>, bool> Environment::pop()
{
    std::pair<std::shared_ptr<ASTNode>, bool> result;
    result.first = m_stack.top();
    m_stack.pop();
    result.second = m_state_stack.top();
    m_state_stack.pop();
    return result;
}

bool Environment::empty()
{
    return m_stack.empty();
}

void Environment::set_send_msg(const std::string &msg)
{
    m_send_msg = msg;
}

void Environment::set_recv_msg(const std::string &msg)
{
    m_recv_msg = msg;
}

const std::string &Environment::get_send_msg() const
{
    return m_send_msg;
}

const std::string &Environment::get_recv_msg() const
{
    return m_recv_msg;
}

} // namespace interpreter