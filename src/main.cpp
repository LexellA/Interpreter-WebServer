#include "server.h"

using namespace server;

int main(int argc, char** argv)
{
    Server server(1705, 3, 60000, true, 8, "root", "localhost", 3306, "root", "18523873534", "chatbot", 65535, 1000);
    server.start();

    return 0;
}