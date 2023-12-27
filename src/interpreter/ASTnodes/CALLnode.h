#pragma once


#include "ASTnode.h"
#include "../func.h"

namespace interpreter
{

class CALLnode : public ASTNode
{
public:
    CALLnode(std::shared_ptr<Environment> env, std::string name);
    ~CALLnode() override;

    void execute() override;

    void get_value(double &value) override { throw std::runtime_error("cannot get number value from call node"); }
    void get_value(std::string &value) override { throw std::runtime_error("cannot get string value from call node"); }
    void get_value(bool &value) override { throw std::runtime_error("cannot get bool value from call node"); }

    std::vector<std::shared_ptr<ASTNode>> get_children()override;

private:
    std::string m_name;

};

} // namespace interpreter