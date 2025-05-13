#include "GameManager.h"

GameManager::GameManager() {

}

std::shared_ptr<Session> GameManager::getSession(SOCKET client) {
    return sessions[client];
}

std::shared_ptr<Room> GameManager::getRoom(const std::string& roomName) {
    return rooms[roomName];
}

void GameManager::Login(SOCKET client, const std::string& name) {
	auto session = std::make_shared<Session>(client);
	session->set_userName(name);
	sessions[client] = session;
}

void GameManager::Enter(SOCKET client, const std::string& roomName) {
    auto session = sessions[client];
    session->set_roomName(roomName);

    // 방이 없다면 새로 생성
    if (rooms.find(roomName) == rooms.end()) {
        rooms[roomName] = std::make_shared<Room>(roomName);
    }

    auto room = rooms[roomName];

    // 빈 자리에 추가
    if (!room->isFull()) {
        room->addPlayer(session);  // 이 함수는 player1/player2 비었는지 보고 등록해야 함
    }
    else {
        // 방이 꽉 찼음
    }
}

void GameManager::Put(SOCKET client, int x, int y) {
    auto session = getSession(client);
    if (!session) return;

    std::string roomName = session->get_roomName();
    auto it = rooms.find(roomName);
    if (it == rooms.end()) return;

    auto room = it->second;
    room->put_rock(session, x, y);
}

void GameManager::Chat(SOCKET client, const std::string& msg) {
    auto session = getSession(client);
    if (!session) return;

    std::string roomName = session->get_roomName();
    auto it = rooms.find(roomName);
    if (it == rooms.end()) return;

    it->second->chat(session, msg);
}

void GameManager::Exit(SOCKET client) {
    auto session = getSession(client);
    if (!session) return;

    std::string roomName = session->get_roomName();
    auto it = rooms.find(roomName);
    if (it == rooms.end()) return;

    auto room = it->second;
    room->exit(session);             // 메시지 전송
    room->delete_player(session);    // 유저 제거

    if (!room->get_player1() && !room->get_player2()) {
        rooms.erase(roomName);       // 방이 비었으면 삭제
    }

    sessions.erase(client);          // 유저도 세션에서 제거 (선택)
}


