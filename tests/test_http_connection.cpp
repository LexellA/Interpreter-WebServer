#include <iostream>
#include <cassert>
#include <string>
#include <fcntl.h>
#include <sys/uio.h>
#include <iostream>
#include "../src/http_connection.h"

using namespace server;

void test_read()
{
    int fd = open("test_request.txt", O_RDONLY);
    struct sockaddr_in addr;
    HTTPConnection connection(fd, addr);

    int saved_errno = 0;
    ssize_t bytesRead = connection.read(&saved_errno);

    connection.parse_request();

    assert(bytesRead > 0);
    assert(saved_errno == 0);
    assert(connection.get_request().get_method() == HTTPMethod::GET);
    assert(connection.get_request().get_path() == "/index.html");
    assert(connection.get_request().get_version() == HTTPVersion::HTTP_11);
    assert(connection.get_request().get_header().size() == size_t(2));
    assert(connection.get_request().get_header().at("Host") == "localhost");
    assert(connection.get_request().get_header().at("User-Agent") == "Mozilla/5.0");

    close(fd);
}

void test_write()
{
    int fd = open("test_response.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    struct sockaddr_in addr;
    HTTPConnection connection(fd, addr);

    HTTPResponse& response = connection.get_response();
    response.init(HTTPStatus::OK, true, "Hello, World!", "txt");
    connection.make_response();


    int saved_errno = 0;
    ssize_t bytesWritten = connection.write(&saved_errno);

    assert(bytesWritten > 0);
    assert(saved_errno == 0);

    close(fd);
}

int main()
{
    test_read();
    test_write();

    std::cout << "HTTPConnection tests passed!" << std::endl;

    return 0;
}