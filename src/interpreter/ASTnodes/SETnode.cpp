#include "SETnode.h"


namespace interpreter
{

SETnode::SETnode(std::shared_ptr<Environment> env, std::string name, std::shared_ptr<ASTNode> expression)
    : ASTNode(env), m_name(name), m_expression(expression)
{
}


SETnode::~SETnode()
{

}


void SETnode::execute()
{
    ValueType type = m_expression->get_type();
    if(type == ValueType::BOOL)
    {
        throw std::runtime_error("Cannot assign a boolean value to a variable");
    }
    else if(type == ValueType::NUMBER)
    {
        double value;
        m_expression->get_value(value);
        m_env->set_variable(m_name, value);
    }
    else if(type == ValueType::STRING)
    {
        std::string value;
        m_expression->get_value(value);
        m_env->set_variable(m_name, value);
    }
    else if(type == ValueType::NONE)
    {
        throw std::runtime_error("unknown type");
    }
}

std::vector<std::shared_ptr<ASTNode>> SETnode::get_children()
{
    return {m_expression};
}

}// namespace interpreter