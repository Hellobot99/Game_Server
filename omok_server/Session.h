#pragma once
#include <string>
#include <WinSock2.h>



class Session 
{
public:	
	Session(SOCKET socket);
	Session(SOCKET socket, std::string userName);
	std::string get_userName();
	std::string get_roomName();
	SOCKET get_socket();
	void set_userName(std::string name);
    void set_roomName(std::string room);
	void set_socket(SOCKET socket);
	
private:
	SOCKET socket;
	std::string userName;
	std::string roomName;	
};
