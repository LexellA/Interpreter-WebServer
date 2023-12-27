#pragma once
#include "../types.h"
#include "../environment.h"
#include <memory>
#include <vector>

namespace interpreter
{

class ASTNode
{
public:
    

    ASTNode(std::shared_ptr<Environment> env): m_env(env), m_type(ValueType::NONE) {}
    virtual ~ASTNode() = default;

    // 执行节点
    virtual void execute() = 0;

    // 获取节点的值
    virtual void get_value(double &value) { throw std::runtime_error("Not implemented, get number value"); }
    virtual void get_value(std::string &value) { throw std::runtime_error("Not implemented, get str value"); }
    virtual void get_value(bool &value) { throw std::runtime_error("Not implemented, get bool value"); }

    // 中断循环
    void set_break_loop() { break_loop = true; }
    bool is_break_loop() { return break_loop; }

    // 获取子节点
    virtual std::vector<std::shared_ptr<ASTNode>> get_children() {
        return {};
    }

    ValueType get_type() { return m_type; }
    void set_type(ValueType type) { m_type = type; }

protected:
    std::shared_ptr<Environment> m_env;

private:
    ValueType m_type;
    bool break_loop = false;
};

} // namespace interpreter