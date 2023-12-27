#pragma once

#include "environment.h"
#include "ASTnodes/ASTnode.h"

#include <memory>
#include <fstream>
#include <sstream>




namespace interpreter
{

class Interpreter
{
public:
    Interpreter(std::shared_ptr<Environment> env);
    ~Interpreter();

    // 解析文件
    void parse(std::string file);

    void input(const std::string &line);
    const std::string &output() const;

    // 开始运行
    void run();

    // 在中断处继续运行
    void continue_run();

    // 是否在运行
    bool is_running() const;

private:
    std::shared_ptr<ASTNode> parse_lines(std::ifstream& fin, std::string line);
    std::shared_ptr<ASTNode> parse_expression(std::stringstream &ss);

    std::shared_ptr<Environment> m_env;
    std::shared_ptr<ASTNode> m_root;
};

} // namespace interpreter