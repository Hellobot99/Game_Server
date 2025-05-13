#include "Server.h"
#include "PacketHandler.h"
#include "GameManager.h"

Server::Server(int port) : port(port), server_fd(-1)
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
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    } 

    int option = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));

    // 2. 주소 설정
    SOCKADDR_IN address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server_fd, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        stop();
        return;
    }

    // 4. 리슨
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        stop();
        return;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    fd_set readfds, tempfds;
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int fd_max = server_fd;
    int num = 0;

    std::vector<SOCKET> client_fds;

    char buffer[BUFFER_SIZE];

    GameManager gameManager;
    PacketHandler packetHandler(&gameManager);

    while (1) {
        tempfds = readfds;
        int activity = select(fd_max + 1, &tempfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &tempfds)) {
            SOCKET temp_client = accept(server_fd, (struct sockaddr*)&address, &addrlen);
            if (temp_client < 0) {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                stop();
            }

            client_fds.push_back(temp_client);

            FD_SET(temp_client, &readfds);

            if (temp_client > fd_max) {
                fd_max = temp_client;
            }

            std::cout << client_fds.size() << " Client connected!" << std::endl;  
        }
        for (auto it = client_fds.begin(); it != client_fds.end(); ) {
            if (FD_ISSET(*it, &tempfds)) {
                int n = recv(*it, buffer, sizeof(buffer), 0);
                if (n == 0) {
                    // 클라이언트 연결 끊김
                    gameManager.Exit(*it);
                    num--;
                    SOCKET client_fd = *it;
                    std::cout << client_fd << " Disconnected.." << std::endl;
                    closesocket(client_fd);
                    it = client_fds.erase(it);  // 클라이언트 목록에서 제거
                }
                else if (n < 0) {
                    // 오류 발생
                    std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
                    closesocket(*it);
                    it = client_fds.erase(it);  // 클라이언트 목록에서 제거
                }
                else {
                    // 클라이언트에게 데이터 전송 받음
                    packetHandler.process(*it, std::string(buffer));
                    ++it;
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

