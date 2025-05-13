#include "PacketHandler.h"

PacketHandler::PacketHandler(GameManager* gameManager) {
    this->gameManager = gameManager;
}

void PacketHandler::process(SOCKET client, const std::string& message) {
    int index = message.find(":");
    if (index == std::string::npos) {
        // 잘못된 형식
        return;
    }

    std::string command = message.substr(0, index);
    std::string payload = message.substr(index + 1);

    if (command == "LOGIN") {
        handleLogin(client, payload);
    }
    else if (command == "ENTER") {
        handleEnter(client, payload);
    }
    else if (command == "PUT") {
        int comma = payload.find(",");
        if (comma != std::string::npos) {
            int x = std::stoi(payload.substr(0, comma));
            int y = std::stoi(payload.substr(comma + 1));
            handlePut(client, x, y);
        }
    }
    else if (command == "CHAT") {
        handleChat(client, payload);
    }
    else if (command == "EXIT") {
        handleExit(client);
    }
    else {
        // 알 수 없는 명령 처리
    }
}

void PacketHandler::handleLogin(SOCKET client, const std::string& name) {
    gameManager->Login(client, name);
    std::cout << name << " Logined" << std::endl;
}

void PacketHandler::handleEnter(SOCKET client, const std::string& roomName) {
    gameManager->Enter(client, roomName);
    std::cout << roomName << " Entered" << std::endl;
}

void PacketHandler::handlePut(SOCKET client, int x, int y) {
    gameManager->Put(client, x, y);
    std::cout << "Put" << x << " , " << y << std::endl;
}

void PacketHandler::handleChat(SOCKET client, const std::string& msg) {
    gameManager->Chat(client, msg);
    std::cout << "Chat" << msg << std::endl;
}

void PacketHandler::handleExit(SOCKET client) {
    gameManager->Exit(client);
    std::cout << "Exit" << std::endl;
}