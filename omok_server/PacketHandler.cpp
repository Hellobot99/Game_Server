#include "PacketHandler.h"

PacketHandler::PacketHandler(GameManager* gameManager) {
    this->gameManager = gameManager;
}

void PacketHandler::process(SOCKET client, const std::string& message) {
    int index = message.find(":");
    if (index == std::string::npos) {
        // �߸��� ����
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
        // �� �� ���� ��� ó��
    }
}

void PacketHandler::handleLogin(SOCKET client, const std::string& name) {

}

void PacketHandler::handleEnter(SOCKET client, const std::string& roomName) {

}

void PacketHandler::handlePut(SOCKET client, int x, int y) {

}

void PacketHandler::handleChat(SOCKET client, const std::string& msg) {

}

void PacketHandler::handleExit(SOCKET client) {

}