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
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        tempfds = readfds;
        int activity = select(fd_max + 1, &tempfds, NULL, NULL, &timeout);
        std::cout << "test" << std::endl;

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
                std::cout << "ASD" << std::endl;
                // 1. 헤더 크기만큼 peek 시도
                int headerSize = 8;
                int peekLen = recv(*it, buffer, headerSize, MSG_PEEK);
                if (peekLen <= 0) {
                    std::cout << "peek failed or disconnected" << std::endl;
                    closesocket(*it);
                    it = client_fds.erase(it);
                    continue;
                }

                // 2. 길이가 충분하지 않으면 skip (패킷 다 안 옴)
                if (peekLen < headerSize) {
                    std::cout << "partial header, wait more" << std::endl;
                    ++it;
                    continue;
                }

                // 3. 헤더에서 총 패킷 길이 추출 (너 구조 기준에 따라 조정)
                int packetSize = *(short*)(buffer + 3);  // ← MsgPackPacketHeaderInfo 기준: 3 offset

                // 4. 총 패킷 길이만큼 peek로 확인
                int totalPeek = recv(*it, buffer, packetSize, MSG_PEEK);
                if (totalPeek < packetSize) {
                    std::cout << "partial packet, wait more" << std::endl;
                    ++it;
                    continue;
                }

                // 5. 이제 안전하게 실제 수신
                int n = recv(*it, buffer, packetSize, 0);
                std::cout << "recv 리턴값: " << n << std::endl;
                
                if (n == 0) {
                    // 클라이언트 연결 끊김
                    gameManager.Exit(*it);
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
                    std::cout << "asd" << std::endl;
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

