#include "func.h"


namespace interpreter
{

Func::Func(std::shared_ptr<ASTNode> root)
    : m_root(root)
{
}

Func::~Func()
{
}

} // namespace interpreter