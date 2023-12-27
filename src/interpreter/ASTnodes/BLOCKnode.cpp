#include "BLOCKnode.h"


namespace interpreter
{

BLOCKnode::BLOCKnode(std::shared_ptr<Environment> env, std::vector<std::shared_ptr<ASTNode>>& nodes)
    : ASTNode(env), m_nodes(nodes)
{
}

BLOCKnode::~BLOCKnode()
{
}

void BLOCKnode::execute()
{
}

std::vector<std::shared_ptr<ASTNode>> BLOCKnode::get_children()
{
    return m_nodes;
}


}// namespace interpreter