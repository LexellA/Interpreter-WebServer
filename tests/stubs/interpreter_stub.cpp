
#include "../../src/interpreter/interpreter.h"


namespace interpreter
{

Interpreter::Interpreter(std::shared_ptr<Environment> env)
    : m_env(env)
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::parse(std::string file)
{
    return;
}

void Interpreter::input(const std::string& line)
{
    return;
}

const std::string& Interpreter::output() const
{
    static std::string s = "simple output";
    return s;
}

void Interpreter::run()
{
    return;
}

void Interpreter::continue_run()
{
    return;
}

bool Interpreter::is_running() const
{
    return false;
}



}