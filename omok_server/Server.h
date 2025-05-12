#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class Server
{
public:
    Server(int port);
    void start();
    void stop();


private:
    int port;
    int server_fd;
};