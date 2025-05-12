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
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    int option = 1;
    int optlen = sizeof(option);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)option, optlen);

    // 2. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    // 3. 바인딩
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        stop();
    }

    // 4. 리슨
    if (listen(server_fd, 1) < 0)
    {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        stop();
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    fd_set readfds, tempfds;
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int fd_max = server_fd;

    std::vector<SOCKET> client_fds;

    //std::unordered_map <std::string, chatRoom> chatRooms;
    //std::unordered_map <std::string, std::string> userNames;

    char buffer[BUFFER_SIZE];

    GameManager game_manager;
    PacketHandler packet_handler(&game_manager);

    while (1) {
        tempfds = readfds;
        int activity = select(fd_max + 1, &tempfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            SOCKET temp_client = accept(server_fd, (struct sockaddr*)&address, &addrlen);
            if (temp_client < 0) {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                stop();
            }

            client_fds.push_back(temp_client);

            if (temp_client > fd_max) {
                fd_max = temp_client;
            }

            std::cout << client_fds.size() << " Client connected!" << std::endl;

        }
        else {
            for (auto it = client_fds.begin(); it != client_fds.end(); it++) {
                if (FD_ISSET(*it, &readfds)) {
                    int n = send(*it, buffer, sizeof(buffer), 0);
                    if (n <= 0) {
                        // 클라이언트 연결 끊김
                        //TODO 게임매니저한테도 보내기
                        SOCKET client_fd = *it;
                        it = client_fds.erase(it);
                        closesocket(client_fd);
                    }
                    else {
                        // 클라이언트에게 데이터 전송 받음
                        
                        packet_handler.process(*it, std::string(buffer));
                    }
                }
                else {
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