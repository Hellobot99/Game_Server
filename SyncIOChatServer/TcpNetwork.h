#pragma once

#define FD_SETSIZE 5096 // http://blog.naver.com/znfgkro1/220175848048

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>


#include <vector>
#include <deque>
#include <unordered_map>

#pragma pack(push, 1)
typedef struct
{
	int cmd;
	int packetType;
	int packetSize;
	char data[1024];
}Packet;
#pragma pack(pop)


namespace NServerNetLib
{
	class TcpNetwork
	{
	public:
		TcpNetwork();
		virtual ~TcpNetwork();

		
		void Run();

		void Stop();
	private:
		int server_fd;
	};
}
