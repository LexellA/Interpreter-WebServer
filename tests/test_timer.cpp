#include <iostream>
#include <cassert>
#include <string>
#include <thread>
#include "../src/lib/timer.h"

using namespace server;

void test_add_timer()
{
    Timer timer;
    int id = timer.add_timer([]() { std::cout << "Timer callback executed!" << std::endl; }, 1000);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    timer.tick();

    assert(id >= 0);
}

void test_del_timer()
{
    Timer timer;
    int id = timer.add_timer([]() { std::cout << "deleted Timer callback executed!" << std::endl; }, 1000);

    timer.del_timer(id);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    timer.tick();
}

void test_reset_timer()
{
    Timer timer;
    int id = timer.add_timer([]() { std::cout << "reseted Timer callback executed!" << std::endl; }, 1000);

    bool result = timer.reset_timer(id, 2000);
    assert(result);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    timer.tick();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    timer.tick();
}

void test_tick()
{
    Timer timer;
    int count = 0;

    timer.add_timer([&count]() { count++; }, 1000);
    timer.add_timer([&count]() { count++; }, 2000);
    timer.add_timer([&count]() { count++; }, 3000);

    std::this_thread::sleep_for(std::chrono::seconds(4));

    timer.tick();

    assert(count == 3);
}

int main()
{
    Log::get_instance().init(Log::Level::DEBUG, "timer_test.txt");
    test_add_timer();
    test_del_timer();
    test_reset_timer();
    test_tick();

    std::cout << "Timer tests passed!" << std::endl;

    return 0;
}