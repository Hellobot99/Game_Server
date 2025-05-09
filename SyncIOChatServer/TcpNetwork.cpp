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
		
    void TcpNetwork::Run(char port_input[])
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
		int fd_max = 0, fd_num;

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

        std::vector<SOCKET> client_fds;
		fd_max = server_fd;

        while (1)
        {
            fd_set read_set;
            FD_ZERO(&read_set);
            FD_SET(server_fd, &read_set);
            for (int client_fd : client_fds) {
                FD_SET(client_fd, &read_set);
            }

            timeval timeout = { 0, 1000 };           

            if ((fd_num = select(fd_max + 1, &read_set, NULL, NULL, &timeout)) < 0) {
                perror("select failed");
                exit(EXIT_FAILURE);
            }

            if (FD_ISSET(server_fd, &read_set)) {
                int temp_client = accept(server_fd, (struct sockaddr*)&address, (int*)&addrlen);
                if (temp_client < 0) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }

                client_fds.push_back(temp_client);

                if (temp_client > fd_max) {
                    fd_max = temp_client;
                }

                std::cout << client_fds.size() << " Client connected!" << std::endl;
            }

            for (auto it = client_fds.begin(); it != client_fds.end(); ) {
                if (FD_ISSET(*it, &read_set)) {
                    int n = recv(*it, (char*)&packet, sizeof(packet), 0);
                    if (n <= 0) {
                        closesocket(*it);
                        it = client_fds.erase(it);
                        std::cout << "Client left, " << client_fds.size() << " Client left" << std::endl;
                    }
                    else {
                        for (auto iter = client_fds.begin(); iter != client_fds.end(); iter++)
                            if (*it != *iter) send(*iter, (const char*)&packet, sizeof(packet), 0);
                        ++it;
                    }
                }
                else ++it;                
            }
        }
    }

	void TcpNetwork::Stop()
	{
		closesocket(server_fd);
		WSACleanup();
	}

}