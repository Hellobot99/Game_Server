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

    // ���� ���ٸ� ���� ����
    if (rooms.find(roomName) == rooms.end()) {
        rooms[roomName] = std::make_shared<Room>(roomName);
    }

    auto room = rooms[roomName];

    // �� �ڸ��� �߰�
    if (!room->isFull()) {
        room->addPlayer(session);  // �� �Լ��� player1/player2 ������� ���� ����ؾ� ��
    }
    else {
        // ���� �� á��
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
    room->exit(session);             // �޽��� ����
    room->delete_player(session);    // ���� ����

    if (!room->get_player1() && !room->get_player2()) {
        rooms.erase(roomName);       // ���� ������� ����
    }

    sessions.erase(client);          // ������ ���ǿ��� ���� (����)
}


