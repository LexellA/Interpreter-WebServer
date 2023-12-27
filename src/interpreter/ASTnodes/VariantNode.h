#pragma once

#include "ASTnode.h"

namespace interpreter
{
    
class VariantNode : public ASTNode
{
public:
    VariantNode(std::shared_ptr<Environment> env, std::string name);
    ~VariantNode() override;

    void execute() override;

    void get_value(double &value) override;
    void get_value(std::string &value) override;
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from variant"); }

private:
    std::string m_name;

    std::variant<double, std::string> m_value;
    
};


} // namespace interpreter
