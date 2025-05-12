#include "Server.h"

Server::Server(int port) : port(port), server_fd(-1), epfd(-1)
{
    // Constructor implementation
}

Server::~Server()
{
    // Destructor implementation
    stop();
}

void Server::start()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int option = 1;
    int optlen = sizeof(option);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

    // 2. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    // 3. 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. 리슨
    if (listen(server_fd, 1) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    fd_set readfds, tempfds;
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int max_fd = server_fd;

    std::vector<int> client_fds;

    while (1) {
        tempfds = readfds;
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            //새로운 클라이언트
            
        }
        else {
            for (auto iter = client_fds.begin(); iter != client_fds.end(); iter++) {
                if (FD_ISSET(*iter, &readfds)) {
                    //기존 클라이언트
                }
            }
        }
    }
    
}

void Server::stop()
{
    closesocket(server_fd);
    
    WSACleanup();
}