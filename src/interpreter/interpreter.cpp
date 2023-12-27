#include "interpreter.h"
#include "ASTnodes/ASTnode.h"
#include "ASTnodes/SETnode.h"
#include "ASTnodes/NUMnode.h"
#include "ASTnodes/VARnode.h"
#include "ASTnodes/BLOCKnode.h"
#include "ASTnodes/CALLnode.h"
#include "ASTnodes/IFnode.h"
#include "ASTnodes/OPnode.h"
#include "ASTnodes/SENDnode.h"
#include "ASTnodes/RECVnode.h"
#include "ASTnodes/STRnode.h"
#include "ASTnodes/VariantNode.h"


#include <fstream>
#include <sstream>
#include <iostream>

#include <typeinfo>

namespace interpreter
{

Interpreter::Interpreter(std::shared_ptr<Environment> env)
    : m_env(env), m_root(nullptr)
{

}


Interpreter::~Interpreter()
{

}

void Interpreter::parse(std::string file)
{
    // 打开文件
    std::ifstream fin(file);

    if(!fin.is_open())
    {
        throw std::runtime_error("Could not open file: " + file);
    }

    // 创建根节点，根节点是一个BLOCK节点
    std::vector<std::shared_ptr<ASTNode>> nodes;
    std::string line;
    while(std::getline(fin, line))
    {
        // 解析一行，得到一个节点
        std::shared_ptr<ASTNode> node = parse_lines(fin, line);
        if(node != nullptr)
        {
            nodes.push_back(node);
        }
    }
    m_root = std::make_shared<BLOCKnode>(m_env, nodes);
}

std::shared_ptr<ASTNode> Interpreter::parse_lines(std::ifstream& fin, std::string line)
{
    // 获得第一个token
    std::stringstream ss(line);
    std::string token;
    ss >> token;

    // 去除空行
    while(token == "")
    {
        std::getline(fin, line);
        ss = std::stringstream(line);
        ss >> token;
    }

    if(token == "VAR") // 变量声明
    {
        std::string name;
        ss >> name;
        std::shared_ptr<ASTNode> exp = parse_expression(ss);
        return std::make_shared<VARnode>(m_env, name, exp);
    }
    else if(token == "SET") // 变量赋值
    {
        std::string name;
        ss >> name;
        std::shared_ptr<ASTNode> exp = parse_expression(ss);
        return std::make_shared<SETnode>(m_env, name, exp);
    }
    else if(token == "CALL") // 函数调用
    {
        std::string name;
        ss >> name;
        return std::make_shared<CALLnode>(m_env, name);
    }
    else if(token == "IF") // if语句
    {
        std::shared_ptr<ASTNode> condition = parse_expression(ss);
        std::vector<std::shared_ptr<ASTNode>> nodes;
        std::string line;
        while(std::getline(fin, line))
        {
            std::shared_ptr<ASTNode> node = parse_lines(fin, line);
            if(node == nullptr)
            {
                break;
            }
            nodes.push_back(node);
        }
        std::shared_ptr<ASTNode> if_block = std::make_shared<BLOCKnode>(m_env, nodes);
        return std::make_shared<IFnode>(m_env, condition, if_block);
    }
    else if(token == "ENDIF") // if结束
    {
        return nullptr;
    }
    else if(token == "SEND") // 发送消息
    {
        std::shared_ptr<ASTNode> exp = parse_expression(ss);
        return std::make_shared<SENDnode>(m_env, exp);
    }
    else if(token == "RECV") // 接收消息
    {
        std::string name;
        ss >> name;
        return std::make_shared<RECVnode>(m_env, name);
    }
    else if(token == "FUNC") // 函数声明
    {
        std::string name;
        ss >> name;
        std::vector<std::shared_ptr<ASTNode>> nodes;
        std::string line;
        while(std::getline(fin, line))
        {
            std::shared_ptr<ASTNode> node = parse_lines(fin, line);
            if(node == nullptr)
            {
                break;
            }
            nodes.push_back(node);
        }
        std::shared_ptr<ASTNode> func_block = std::make_shared<BLOCKnode>(m_env, nodes);
        std::shared_ptr<Func> func = std::make_shared<Func>(func_block);
        m_env->add_function(name, func);
        return nullptr;
    }
    else if(token == "ENDFUNC") // 函数结束
    {
        return nullptr;
    }

    throw std::runtime_error("Unknown token: " + token);
}

std::shared_ptr<ASTNode> Interpreter::parse_expression(std::stringstream& ss)
{
    // 获得第一个token
    std::string token;
    ss >> token;

    OPnode::Operator op;
    op = OPnode::get_op(token);
    if(op != OPnode::Operator::UNKNOWN)
    {
        // 如果是运算符，那么就是一个OPnode，解析两个表达式
        std::shared_ptr<ASTNode> left = parse_expression(ss);
        std::shared_ptr<ASTNode> right = parse_expression(ss);
        return std::make_shared<OPnode>(m_env, op, left, right);
    }
    else if(token[0] == '"')
    {
        // 如果是字符串，那么就是一个STRnode
        std::string temp;
        while(token[token.size() - 1] != '"')
        {
            ss >> temp;
            token += " " + temp;
        }
        return std::make_shared<STRnode>(m_env, token.substr(1, token.size() - 2));
    }

    try
    {
        // 如果是数字，那么就是一个NUMnode
        double value = std::stod(token);
        return std::make_shared<NUMnode>(m_env, value);
    }
    catch(const std::invalid_argument& e)
    {
        // 否则就是一个变量，VARnode
        return std::make_shared<VariantNode>(m_env, token);
    }
    
}

void Interpreter::run()
{
    try 
    {
        m_env->push(m_root, false);
        continue_run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Interpreter::continue_run()
{
    while(!m_env->empty())
    {
        // 从栈中取出一个节点，执行它
        auto res = m_env->pop();
        std::shared_ptr<ASTNode> node = res.first;
        if(node == nullptr)
        {
            continue;
        }
        // 如果需要收消息，那么就中断
        bool state = res.second;
        if(node->is_break_loop() && state == false)
        {
            m_env->push(node, true);
            break;
        }

        //将节点的子节点压入栈中，节点状态改为true压栈
        if(state == false)
        {
            m_env->push(node, true);
            auto children = node->get_children();
            for(auto it = children.rbegin(); it != children.rend(); ++it)
            {
                m_env->push(*it, false);
            }
        }
        else
        {
            node->execute();
        }
    }
}


void Interpreter::input(const std::string &line)
{
    m_env->set_recv_msg(line);
}

const std::string &Interpreter::output() const
{
    return m_env->get_send_msg();
}

bool Interpreter::is_running() const
{
    return !m_env->empty();
}

} // namespace interpreter