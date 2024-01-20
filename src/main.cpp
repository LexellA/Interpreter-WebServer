#include "server.h"

using namespace server;

int main(int argc, char** argv)
{
    Server server(3411, 3, 60000, true, 8, 
                "root", "localhost", 3306, "root", "123456", "chatbot",
                15000, 1000, Log::Level::DEBUG, "./scripts/script_store.txt");

    server.start();

    return 0;
}