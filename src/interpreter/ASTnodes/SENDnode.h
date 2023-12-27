#pragma once

#include "ASTnode.h"


namespace interpreter
{

class SENDnode : public ASTNode
{
public:
    SENDnode(std::shared_ptr<Environment> env, std::shared_ptr<ASTNode> expression);
    ~SENDnode() override;

    void execute() override;

    std::vector<std::shared_ptr<ASTNode>> get_children() override;

private:
    std::shared_ptr<ASTNode> m_expression;

};

}