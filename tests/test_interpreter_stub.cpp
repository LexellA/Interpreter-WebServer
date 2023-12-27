#include "../src/interpreter/interpreter.h"
#include <cassert>
#include <iostream>

using namespace interpreter;

//使用environment_stub.cpp进行测试
int main()
{
    Interpreter interpreter(std::make_shared<Environment>());
    interpreter.parse("test_scripts/test1.txt");
    interpreter.run();
    interpreter.input("world");
    interpreter.continue_run();
    assert(interpreter.output() == "send_msg");
    return 0;
}