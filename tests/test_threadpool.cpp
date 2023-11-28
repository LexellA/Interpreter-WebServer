#include <iostream>
#include <cassert>
#include "../src/threadpool.h"

void test_threadpool()
{
    server::ThreadPool threadpool;

    // Test adding tasks to the threadpool
    auto task1 = threadpool.add_task([]() { return 1; });
    auto task2 = threadpool.add_task([](int a, int b) { return a + b; }, 2, 3);
    auto task3 = threadpool.add_task([](const std::string& str) { return str.size(); }, "Hello, world!");

    // Test getting results from the tasks
    assert(task1.get() == 1);
    assert(task2.get() == 5);
    assert(task3.get() == 13);

    std::cout << "threadpool tests passed!" << std::endl;
}

int main()
{
    test_threadpool();

    return 0;
}