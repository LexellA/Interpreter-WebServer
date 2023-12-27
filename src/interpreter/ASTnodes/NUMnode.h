#pragma once

#include "ASTnode.h"

namespace interpreter
{

class NUMnode : public ASTNode
{
public:
    NUMnode(std::shared_ptr<Environment> env, double value);
    ~NUMnode() override;

    void execute() override;

    void get_value(double &value) override;
    void get_value(std::string &value) override { throw std::runtime_error("cannot get string value from number node"); }
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from number node"); }
    
private:
    double m_value;

};

} // namespace interpreter
