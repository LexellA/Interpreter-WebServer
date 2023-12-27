#pragma once

#include "ASTnode.h"

namespace interpreter
{

class STRnode : public ASTNode
{
public:
    STRnode(std::shared_ptr<Environment> env, std::string value);
    ~STRnode() override;

    void execute() override;

    void get_value(double &value) override { throw std::runtime_error("cannot get number value from string node"); }
    void get_value(std::string &value) override;
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from string node"); }
private:
    std::string m_value;
};

} // namespace interpreter
