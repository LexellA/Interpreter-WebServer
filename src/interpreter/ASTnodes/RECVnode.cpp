#include "RECVnode.h"

namespace interpreter
{
RECVnode::RECVnode(std::shared_ptr<Environment> env, std::string name)
    : ASTNode(env), m_name(name)
{
    set_break_loop();
}

RECVnode::~RECVnode()
{
}

void RECVnode::execute()
{
    std::string value = m_env->get_recv_msg();
    try
    {
        double num = std::stod(value);
        m_env->set_variable(m_name, num);
        set_type(ValueType::NUMBER);
        return;
    }
    catch (const std::exception& e)
    {
        m_env->set_variable(m_name, value);
        set_type(ValueType::STRING);
        return;
    }
}

} // namespace interpreter
