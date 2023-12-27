#include "NUMnode.h"



namespace interpreter
{

NUMnode::NUMnode(std::shared_ptr<Environment> env, double value)
    : ASTNode(env), m_value(value)
{

}

NUMnode::~NUMnode()
{
}

void NUMnode::execute()
{
    set_type(ValueType::NUMBER);
}

void NUMnode::get_value(double &value)
{
    value = m_value;
}


} // namespace interpreter