#pragma once

#include "BLOCKnode.h"

namespace interpreter
{
class IFnode : public ASTNode
{
public:
    IFnode(std::shared_ptr<Environment> env, std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> if_block);
    ~IFnode() override;

    void execute() override;

    void get_value(double &value) override { throw std::runtime_error("cannot get double value from if node"); }
    void get_value(std::string &value) override { throw std::runtime_error("cannot get string value from if node"); }
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from if node"); }

    std::vector<std::shared_ptr<ASTNode>> get_children() override;

private:
    std::shared_ptr<ASTNode> m_condition;
    std::shared_ptr<ASTNode> m_if_block;


};

}// namespace interpreter