#include "VariantNode.h"

namespace interpreter
{

VariantNode::VariantNode(std::shared_ptr<Environment> env, std::string name)
    : ASTNode(env), m_name(name)
{
}

VariantNode::~VariantNode()
{
}

void VariantNode::execute()
{
    ValueType type;
    m_env->get_vartype(m_name, type);

    set_type(type);
    if (type == ValueType::NUMBER)
    {
        double value;
        m_env->get_variable(m_name, value);
        m_value = value;
    }
    else if (type == ValueType::STRING)
    {
        std::string value;
        m_env->get_variable(m_name, value);
        m_value = value;
    }
    else
    {
        throw std::runtime_error("unknown type");
    }
}

void VariantNode::get_value(double &value)
{
    if (get_type() == ValueType::NUMBER)
    {
        value = std::get<double>(m_value);
    }
    else
    {
        throw std::runtime_error("cannot get double value from a string variant");
    }
}

void VariantNode::get_value(std::string &value)
{
    if (get_type() == ValueType::STRING)
    {
        value = std::get<std::string>(m_value);
    }
    else
    {
        throw std::runtime_error("cannot get string value from a number variant");
    }
}

} // namespace interpreter