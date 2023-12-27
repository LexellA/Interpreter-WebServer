
#include "IFnode.h" 


namespace interpreter
{

IFnode::IFnode(std::shared_ptr<Environment> env, std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> if_block)
    : ASTNode(env), m_condition(condition), m_if_block(if_block)
{
}

IFnode::~IFnode()
{
}

void IFnode::execute()
{
    //判断条件类型
    ValueType type = m_condition->get_type();
    if(type != ValueType::BOOL)
    {
        throw std::runtime_error("if condition must be bool");
    }

    //判断条件
    bool condition;
    m_condition->get_value(condition);
    if(condition)
    {
        m_env->push(m_if_block, false);
    }
}

std::vector<std::shared_ptr<ASTNode>> IFnode::get_children()
{
    return {m_condition};
}

} // namespace interpreter