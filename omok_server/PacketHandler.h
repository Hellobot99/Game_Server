#pragma once
#include <string>
#include <unordered_map>
#include <winsock2.h>
#include "GameManager.h"

class PacketHandler {
public:
    PacketHandler(GameManager* gameManager);

    void process(SOCKET client, const std::string& message);

private:
    GameManager* gameManager;

    void handleLogin(SOCKET client, const std::string& name);
    void handleEnter(SOCKET client, const std::string& roomName);
    void handlePut(SOCKET client, int x, int y);
    void handleChat(SOCKET client, const std::string& msg);
    void handleExit(SOCKET client);
};