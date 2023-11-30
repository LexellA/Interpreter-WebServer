#include <iostream>
#include <thread>
#include <cppconn/statement.h>
#include <mutex>
#include "../src/db_connectionpool.h"

using namespace server;

std::mutex m;

void put_data(DBConnectionPool &connectionPool, int threadId)
{
    try
    {
        auto connection = connectionPool.get_connection();

        auto statement = connection->createStatement();
        auto res = statement->executeQuery("SELECT * FROM user");

        std::lock_guard<std::mutex> lock(m);
        std::cout << "Thread " << threadId << " has got data to the database..." << std::endl;
        while (res->next())
        {
            std::cout << res->getString("username") << std::endl;
            std::cout << res->getString("password") << std::endl;
        }

    }
    catch (sql::SQLException &e)
    {
        std::lock_guard<std::mutex> lock(m);
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        abort();
    }
}

void test_db_connectionpool()
{
    const std::string host = "localhost";
    const std::string user = "root";
    const std::string password = "18523873534";
    const std::string db_name = "chatbot";
    const int port = 3306;
    const int max_connections = 8;

    DBConnectionPool connectionPool(host, user, password, db_name, port, max_connections);

    std::vector<std::thread> threads;

    // Create multiple threads to write data to the database
    for (int i = 0; i < 100; ++i)
    {
        threads.emplace_back(put_data, std::ref(connectionPool), i);
    }

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << "db_connectionpool tests passed!" << std::endl;
}

int main()
{
    test_db_connectionpool();

    return 0;
}