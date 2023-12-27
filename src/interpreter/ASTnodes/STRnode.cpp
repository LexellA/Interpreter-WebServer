#include "STRnode.h"



namespace interpreter
{

STRnode::STRnode(std::shared_ptr<Environment> env, std::string value)
    : ASTNode(env), m_value(value)
{
}

STRnode::~STRnode()
{
}

void STRnode::execute()
{
    set_type(ValueType::STRING);
}

void STRnode::get_value(std::string &value)
{
    value = m_value;
}


} // namespace interpreter