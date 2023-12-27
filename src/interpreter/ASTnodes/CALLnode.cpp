#include "CALLnode.h"

namespace interpreter
{
    
CALLnode::CALLnode(std::shared_ptr<Environment> env, std::string name)
    : ASTNode(env), m_name(name)
{
}

CALLnode::~CALLnode()
{
}


void CALLnode::execute()
{
}

std::vector<std::shared_ptr<ASTNode>> CALLnode::get_children()
{
    return {m_env->get_function(m_name)->get_root()};
}

} // namespace interpreter
