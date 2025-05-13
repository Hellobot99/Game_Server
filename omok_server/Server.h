#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <winsock.h>
#include <unordered_set>
#include <unordered_map>

#define BUFFER_SIZE 1024

class Server
{
public:
    Server(int port);
    ~Server();
    void start();
    void stop();


private:
    int port;
    SOCKET server_fd;
};