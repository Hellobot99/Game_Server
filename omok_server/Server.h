#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>

class Server
{
public:
    Server(int port);
    void start();
    void stop();
}

private:
    int port;
    int server_fd;
    int epfd;
};