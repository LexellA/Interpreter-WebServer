#include "../src/interpreter/interpreter.h"
#include <cassert>
#include <iostream>

using namespace interpreter;

void test1()
{
    Interpreter interpreter(std::make_shared<Environment>());
    interpreter.parse("test_scripts/test1.txt");
    interpreter.run();
    interpreter.input(" world");
    interpreter.continue_run();
    assert(interpreter.output() == "hello world");
    std::cout << "test1 passed" << std::endl;
}

void test2()
{
    Interpreter interpreter(std::make_shared<Environment>());
    interpreter.parse("test_scripts/test2.txt");

    // 测试查询余额
    interpreter.run();
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "你好，请问有什么可以帮助你：查询，充值");
    interpreter.input("查询");
    interpreter.continue_run();
    assert(interpreter.output() == "您当前余额为：0.000000，输入任意消息返回，输入退出来退出");
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "你好，请问有什么可以帮助你：查询，充值");

    // 测试充值
    interpreter.input("充值");
    interpreter.continue_run();
    assert(interpreter.output() == "请输入你要充值的数额");
    interpreter.input("100");
    interpreter.continue_run();
    assert(interpreter.output() == "充值成功，输入任意消息返回，输入退出来退出");

    // 再次查询余额
    interpreter.input("查询");
    interpreter.continue_run();
    assert(interpreter.output() == "你好，请问有什么可以帮助你：查询，充值");
    interpreter.continue_run();
    assert(interpreter.output() == "您当前余额为：100.000000，输入任意消息返回，输入退出来退出");
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "你好，请问有什么可以帮助你：查询，充值");

    //测试错误输入
    interpreter.input("启动");
    interpreter.continue_run();
    assert(interpreter.output() == "请检查您的输入，输入任意消息返回，输入退出来退出");

    // 测试退出
    interpreter.input("退出");
    interpreter.continue_run();
    std::cout << "test2 passed" << std::endl;
}

void test3()
{
    Interpreter interpreter(std::make_shared<Environment>());
    interpreter.parse("test_scripts/test3.txt");

    // 测试开始
    interpreter.run();
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "欢迎使用商品查询系统，请输入您要进行的操作：查询，特价，退出");

    // 测试查询手机价格
    interpreter.input("查询");
    interpreter.continue_run();
    assert(interpreter.output() == "请输入您要查询的商品：手机，平板电脑，电脑");
    interpreter.input("手机");
    interpreter.continue_run();
    assert(interpreter.output() == "手机的价格为：3799.000000，任意输入返回");
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "欢迎使用商品查询系统，请输入您要进行的操作：查询，特价，退出");

    // 测试查询特价商品
    interpreter.input("特价");
    interpreter.continue_run();
    assert(interpreter.output() == "请输入您要查询的商品：switch，游戏机");
    interpreter.input("switch");
    interpreter.continue_run();
    assert(interpreter.output() == "switch的价格为：2299.000000，任意输入返回");
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "欢迎使用商品查询系统，请输入您要进行的操作：查询，特价，退出");

    // 测试查询折扣
    interpreter.input("折扣");
    interpreter.continue_run();
    assert(interpreter.output() == "当前所有商品均有9折优惠，任意输入返回");
    interpreter.input("你好");
    interpreter.continue_run();
    assert(interpreter.output() == "欢迎使用商品查询系统，请输入您要进行的操作：查询，特价，退出");

    // 测试退出
    interpreter.input("退出");
    interpreter.continue_run();
    std::cout << "test3 passed" << std::endl;
}

int main()
{
    test1();
    test2();
    test3();
    return 0;
}