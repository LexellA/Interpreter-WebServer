#include <iostream>
#include <cassert>
#include <string>
#include <iostream>
#include "../src/buffer.h"

using namespace server;

void test_push_back()
{
    Buffer buffer(10);
    std::string str = "Hello, World!";
    buffer.push_back(str);

    assert(buffer.available() == str.size());
    assert(buffer.capacity() > 0);
    assert(std::string(buffer.data()) == str);
}

void test_pop_front()
{
    Buffer buffer(10);
    std::string str = "Hello, World!";
    buffer.push_back(str);

    buffer.pop_front(7);

    assert(buffer.available() == str.size() - 7);
    assert(buffer.capacity() > 0);
    assert(std::string(buffer.data()) == "World!");
}

void test_clear()
{
    Buffer buffer(10);
    std::string str = "Hello, World!";
    buffer.push_back(str);

    buffer.clear();

    assert(buffer.available() == 0);
    assert(buffer.capacity() > 0);
    assert(std::string(buffer.data()) == "");
}

void test_edge()
{
    Buffer buffer(1);
    std::string str = "H";

    buffer.push_back(str);

}

int main()
{
    test_push_back();
    test_pop_front();
    test_clear();
    test_edge();

    std::cout << "Buffer tests passed!" << std::endl;

    return 0;
}