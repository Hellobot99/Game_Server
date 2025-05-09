#include <iostream>
#include <thread>
#include "TcpNetwork.h"

int main()
{
	NServerNetLib::TcpNetwork serverNet;

	char port[16] = "9190";
	
	std::thread logicThread([&]() { 		
		serverNet.Run(port); }
	);
	
	std::cout << "press any key to exit...";
	getchar();

	serverNet.Stop();
	logicThread.join();

	return 0;
}