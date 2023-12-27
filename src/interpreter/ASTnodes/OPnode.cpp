#include "OPnode.h"

namespace interpreter
{


std::unordered_map<std::string, OPnode::Operator> OPnode::m_op_map = {
    {"ADD", Operator::ADD},
    {"SUB", Operator::SUB},
    {"MUL", Operator::MUL},
    {"DIV", Operator::DIV},
    {"EQ", Operator::EQ},
    {"LS", Operator::LS},
    {"GT", Operator::GT},
    {"LE", Operator::LE},
    {"GE", Operator::GE},
    {"NE", Operator::NE}
};


    
OPnode::OPnode(std::shared_ptr<Environment> env, Operator op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
    : ASTNode(env), m_op(op), m_left(left), m_right(right)
{
}


OPnode::~OPnode()
{
}

void OPnode::execute()
{

    switch (m_op)
    {
    case Operator::ADD:
        execute_add();
        break;

    case Operator::SUB:
        execute_sub();
        break;

    case Operator::MUL:
        execute_mul();
        break;

    case Operator::DIV:
        execute_div();
        break;

    case Operator::EQ:
        execute_eq();
        break;

    case Operator::LS:
        execute_ls();
        break;

    case Operator::GT:
        execute_gt();
        break;

    case Operator::LE:
        execute_le();
        break;

    case Operator::GE:
        execute_ge();
        break;

    case Operator::NE:
        execute_ne();
        break;
    }
}

void OPnode::execute_add()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::NUMBER);
        m_value = left_value + right_value;
    }
    else if (left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::STRING);
        m_value = left_value + right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::NUMBER)
    {
        std::string left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::STRING);
        m_value = left_value + std::to_string(right_value);
    }
    else if(left_type == ValueType::NUMBER && right_type == ValueType::STRING)
    {
        double left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::STRING);
        m_value = std::to_string(left_value) + right_value;
    }
    else
    {
        throw std::runtime_error("Invalid operands for operator 'ADD'");
    }
}

void OPnode::execute_sub()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::NUMBER);
        m_value = left_value - right_value;
    }
    else
    {
        throw std::runtime_error("Invalid operands for operator 'SUB'");
    }

}

void OPnode::execute_mul()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::NUMBER);
        m_value = left_value * right_value;
    }
    else
    {
        throw std::runtime_error("Invalid operands for operator 'MUL'");
    }
}

void OPnode::execute_div()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::NUMBER);
        m_value = left_value / right_value;
    }
    else
    {
        throw std::runtime_error("Invalid operands for operator 'DIV'");
    }
}

void OPnode::execute_eq()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value == right_value;
    }
    else if (left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value == right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = false;
    }
}

void OPnode::execute_ls()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value < right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value < right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = false;
    }
}

void OPnode::execute_gt()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if (left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value > right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);

        set_type(ValueType::BOOL);
        m_value = left_value > right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = false;
    }
}

void OPnode::execute_le()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if(left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value <= right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value <= right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = false;
    }
}

void OPnode::execute_ge()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if(left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value >= right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value >= right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = false;
    }
}

void OPnode::execute_ne()
{
    ValueType left_type = m_left->get_type();
    ValueType right_type = m_right->get_type();

    if(left_type == ValueType::NUMBER && right_type == ValueType::NUMBER)
    {
        double left_value;
        double right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value != right_value;
    }
    else if(left_type == ValueType::STRING && right_type == ValueType::STRING)
    {
        std::string left_value;
        std::string right_value;

        m_left->get_value(left_value);
        m_right->get_value(right_value);
        
        set_type(ValueType::BOOL);
        m_value = left_value != right_value;
    }
    else
    {
        set_type(ValueType::BOOL);
        m_value = true;
    }
}

std::vector<std::shared_ptr<ASTNode>> OPnode::get_children()
{
    return {m_left, m_right};
}

} // namespace interpreter
