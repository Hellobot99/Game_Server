#include "Room.h"

// 생성자
Room::Room(const std::string& name) {
    this->name = name;
    resetBoard();
    num = 0;
}

// Getter
std::string Room::get_name() const {
    return name;
}

std::shared_ptr<Session> Room::get_player1() {
    return player1;
}

std::shared_ptr<Session> Room::get_player2() {
    return player2;
}

int Room::get_turn() const {
    return turn;
}

int Room::get_board_value(int x, int y) const {
    return board[y][x];
}

// Setter
void Room::set_name(const std::string& n) {
    name = n;
}

void Room::set_turn(int t) {
    turn = t;
}

void Room::set_board_value(int x, int y, int value) {
    board[y][x] = value;
}

// Logic
bool Room::isFull() const {
    return num == 2;
}

void Room::addPlayer(std::shared_ptr<Session> session) {
    if (num == 0) {
        player1 = session;
        ++num;
    }
    else if (num == 1) {
        player2 = session;
        ++num;
    }
    // else 방이 꽉 찼음 → 아무것도 하지 않음 or 에러 처리 가능
}

void Room::resetBoard() {
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            board[i][j] = 0;
}

void Room::put_rock(std::shared_ptr<Session> player, int x, int y) {
    int color = (player == player1) ? 1 : 2;
    board[y][x] = color;
    turn = (turn == 1) ? 2 : 1;
}

void Room::chat(std::shared_ptr<Session> session, const std::string& msg) {
    auto p1 = get_player1();
    auto p2 = get_player2();

    if (!p1 || !p2) return;

    const char* data = msg.c_str();
    int len = msg.length();  // 정확한 길이만큼 전송

    if (session == p1) {
        send(p2->get_socket(), data, len, 0);
    }
    else if (session == p2) {
        send(p1->get_socket(), data, len, 0);
    }
}

void Room::exit(std::shared_ptr<Session> session) {
    if (!player1 || !player2) return;

    std::string msg;
    if (session == player1) {
        msg = "INFO:p1 has left the game";
        send(player2->get_socket(), msg.c_str(), msg.length(), 0);
    }
    else if (session == player2) {
        msg = "INFO:p2 has left the game";
        send(player1->get_socket(), msg.c_str(), msg.length(), 0);
    }
}

void Room::delete_player(std::shared_ptr<Session> session) {
    if (player1 == session) {
        player1 = nullptr;
        --num;
    }
    else if (player2 == session) {
        player2 = nullptr;
        --num;
    }
}