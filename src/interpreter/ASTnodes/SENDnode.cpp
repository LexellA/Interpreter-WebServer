#include "SENDnode.h"


namespace interpreter
{

SENDnode::SENDnode(std::shared_ptr<Environment> env, std::shared_ptr<ASTNode> expression)
    : ASTNode(env), m_expression(expression)
{
}

SENDnode::~SENDnode()
{
}

void SENDnode::execute()
{
    ValueType type = m_expression->get_type();
    if(type == ValueType::STRING)
    {
        std::string value;
        m_expression->get_value(value);
        m_env->set_send_msg(value);    
    }
    else if(type == ValueType::NUMBER)
    {
        double value;
        m_expression->get_value(value);
        m_env->set_send_msg(std::to_string(value));
    }
    else
    {
        throw std::runtime_error("SENDnode: expression is not a string or a number");
    }
}

std::vector<std::shared_ptr<ASTNode>> SENDnode::get_children()
{
    return {m_expression};
}

} // namespace interpreter