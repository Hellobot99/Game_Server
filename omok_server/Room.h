#pragma once
#include <string>
#include <memory>
#include "Session.h"

#define BUFFER_SIZE 1024

class Room
{
public:
    Room(const std::string& name);

    // Getter
    std::string get_name() const;
    std::shared_ptr<Session> get_player1();
    std::shared_ptr<Session> get_player2();
    int get_turn() const;
    int get_board_value(int x, int y) const;

    // Setter
    void set_name(const std::string& name);
    void set_turn(int t);
    void set_board_value(int x, int y, int value);

    // Logic
    void resetBoard();
    void put_rock(std::shared_ptr<Session> player, int x, int y);
    bool isFull() const;
    void addPlayer(std::shared_ptr<Session> session);
    void chat(std::shared_ptr<Session> session, const std::string& msg);
    void exit(std::shared_ptr<Session> session);
    void delete_player(std::shared_ptr<Session> session);

private:
    std::string name;
    std::shared_ptr<Session> player1;
    std::shared_ptr<Session> player2;
    int board[15][15] = { 0 };
    int turn = 1;
    int num = 0;
};
