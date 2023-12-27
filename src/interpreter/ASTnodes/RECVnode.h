#pragma once

#include "ASTnode.h"

namespace interpreter
{

class RECVnode : public ASTNode
{
public:
    RECVnode(std::shared_ptr<Environment> env, std::string name);
    ~RECVnode() override;

    void execute() override;

private:
    std::string m_name;
};

} // namespace interpreter