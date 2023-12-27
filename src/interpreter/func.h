#pragma once

#include "ASTnodes/ASTnode.h"



namespace interpreter
{

class Func
{
public:
    Func(std::shared_ptr<ASTNode> root);
    ~Func();

    std::shared_ptr<ASTNode> get_root() { return m_root; }

private:
    std::shared_ptr<ASTNode> m_root;

};

} // namespace interpreter
