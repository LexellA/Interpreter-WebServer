#include "server.h"

using namespace server;

int main(int argc, char** argv)
{
    Server server(3411, 3, 60000, true, 8, "root", "localhost", 3306, "root", "18523873534", "chatbot", 15000, 1000, Log::Level::DEBUG);
    server.start();

    return 0;
}