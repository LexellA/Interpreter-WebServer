#pragma once

#include "ASTnode.h"

#include <variant>

namespace interpreter
{

class OPnode : public ASTNode
{
public:
    enum class Operator
    {
        ADD,
        SUB,
        MUL,
        DIV,
        EQ,
        LS,
        GT,
        LE,
        GE,
        NE,
        UNKNOWN
    };

    OPnode(std::shared_ptr<Environment> env, Operator op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
    ~OPnode() override;

    void execute() override;

    void get_value(double &value){value = std::get<double>(m_value);}
    void get_value(std::string &value){value = std::get<std::string>(m_value);}
    void get_value(bool &value){value = std::get<bool>(m_value);}


    std::vector<std::shared_ptr<ASTNode>> get_children()override;

    static Operator get_op(std::string op)
    {
        auto it = m_op_map.find(op);
        if(it != m_op_map.end())
        {
            return it->second;
        }
        else
        {
            return Operator::UNKNOWN;
        }
    }


private:
    void execute_add();
    void execute_sub();
    void execute_mul();
    void execute_div();
    void execute_eq();
    void execute_ls();
    void execute_gt();
    void execute_le();
    void execute_ge();
    void execute_ne();

    Operator m_op;
    std::shared_ptr<ASTNode> m_left;
    std::shared_ptr<ASTNode> m_right;

    std::variant<double, std::string, bool> m_value;

    static std::unordered_map<std::string, Operator> m_op_map;

};

} // namespace interpreter