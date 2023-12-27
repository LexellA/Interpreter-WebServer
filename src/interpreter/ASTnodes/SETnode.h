#pragma once

#include "ASTnode.h"

namespace interpreter
{

class SETnode : public ASTNode
{
public:
    SETnode(std::shared_ptr<Environment> env, std::string name, std::shared_ptr<ASTNode> expression);
    ~SETnode() override;

    void execute() override;

    std::vector<std::shared_ptr<ASTNode>> get_children() override;
private:
    std::string m_name;
    std::shared_ptr<ASTNode> m_expression;
};

} // namespace interpreter