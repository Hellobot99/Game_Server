#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <unordered_set>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib")

#define JOIN 1
#define MESSAGE 2
#define EXIT 3

#define BUFFER_SIZE 1024

#pragma pack(push,1)
typedef struct {
    std::string name;
    std::unordered_set<int> clients;
} chatRoom;
#pragma pack(pop)

class Server
{
public:
    Server(int port);
    void start();
    void stop();


private:
    int port;
    SOCKET server_fd;
};