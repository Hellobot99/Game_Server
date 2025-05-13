#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include "Session.h"
#include "Room.h"

class GameManager {
public:
    GameManager();

    void Login(SOCKET client, const std::string& name);
    void Enter(SOCKET client, const std::string& roomName);
    void Put(SOCKET client, int x, int y);
    void Chat(SOCKET client, const std::string& msg);
    void Exit(SOCKET client);

    std::shared_ptr<Session> getSession(SOCKET client);
    std::shared_ptr<Room> getRoom(const std::string& roomName);

private:
    std::unordered_map<SOCKET, std::shared_ptr<Session>> sessions;
    std::unordered_map<std::string, std::shared_ptr<Room>> rooms;
};
