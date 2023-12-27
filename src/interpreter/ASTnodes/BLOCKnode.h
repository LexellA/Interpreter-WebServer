#pragma once

#include "ASTnode.h"
#include <vector>

namespace interpreter
{

class BLOCKnode : public ASTNode
{
public:
    BLOCKnode(std::shared_ptr<Environment> env, std::vector<std::shared_ptr<ASTNode>>& nodes);
    ~BLOCKnode() override;

    void execute() override;

    void get_value(double &value) override { throw std::runtime_error("cannot get double value from block node"); }
    void get_value(std::string &value) override { throw std::runtime_error("cannot get string value from block node"); }
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from block node"); }

    std::vector<std::shared_ptr<ASTNode>> get_children() override;

private:
    // 子节点
    std::vector<std::shared_ptr<ASTNode>> m_nodes;
};

}// namespace interpreter