#pragma once

#include "types.h"

#include <string>
#include <unordered_map>
#include <variant>
#include <stack>
#include <memory>

namespace interpreter
{
class Func;
class ASTNode;

class Environment
{
public:
    Environment();
    ~Environment();

    void add_variable(const std::string& name, double value);
    void add_variable(const std::string& name, const std::string& value);
    void add_function(const std::string& name, std::shared_ptr<Func> func);

    void set_variable(const std::string& name, double value);
    void set_variable(const std::string& name, const std::string& value);

    void get_vartype(const std::string& name, ValueType& type);

    void get_variable(const std::string& name, double& value);
    void get_variable(const std::string& name, std::string& value);

    std::shared_ptr<Func> get_function(const std::string& name);

    void set_send_msg(const std::string &msg);
    void set_recv_msg(const std::string &msg);
    const std::string &get_send_msg() const;
    const std::string &get_recv_msg() const;

    void push(std::shared_ptr<ASTNode> node, bool state);
    std::pair<std::shared_ptr<ASTNode>, bool> pop();
    bool empty();
private:
    std::unordered_map<std::string, ValueType> m_types; // 变量类型
    std::unordered_map<std::string, std::variant<double, std::string>> m_variables; // 变量值
    std::unordered_map<std::string, std::shared_ptr<Func>> m_functions; // 函数

    std::stack<std::shared_ptr<ASTNode>> m_stack; // 语句栈
    std::stack<bool> m_state_stack; // 语句栈状态
    std::string m_send_msg; // 发送的消息
    std::string m_recv_msg; // 接收的消息
};

} // namespace interpreter