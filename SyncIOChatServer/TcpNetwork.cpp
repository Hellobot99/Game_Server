#include <iostream>
#include <cstring>
#include <vector>
#include <deque>

#include "TcpNetwork.h"

namespace NServerNetLib
	 
{
	TcpNetwork::TcpNetwork()
	{		
	}
	
	TcpNetwork::~TcpNetwork() 
	{		
	}
		
    void TcpNetwork::Run()
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return;
        }

        int port = std::atoi(port_input);
        sockaddr_in address;
        int addrlen = sizeof(address);
		Packet packet;

        // 소켓 생성
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == INVALID_SOCKET) {
            std::cerr << "Socket creation failed\n";
            WSACleanup();
            return;
        }

        // setsockopt (bind 전에 해야 함)
        int option = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));

        // 주소 설정
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // 바인딩
        if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
            std::cerr << "Bind failed\n";
            closesocket(server_fd);
            WSACleanup();
            return;
        }

        // 리슨
        if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed\n";
            closesocket(server_fd);
            WSACleanup();
            return;
        }

        std::cout << "Listening on port " << port << "...\n";        

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.
        int epfd = epoll_create1(0), npfd;

    }

	void TcpNetwork::Stop()
	{
		closesocket(server_fd);
		WSACleanup();
	}

}