#include "Session.h"

Session::Session(SOCKET socket) {
	this->socket = socket;
	this->userName = "";
	this->roomName = "";
}

Session::Session(SOCKET socket, std::string userName) {
	this->socket = socket;
	this->userName = userName;
	this->roomName = "";
}

std::string Session::get_userName() {
	return userName;
}

std::string Session::get_roomName() {
	return roomName;
}

SOCKET Session::get_socket() {
	return socket;
}

void Session::set_userName(std::string name) {
	userName = name;
}

void Session::set_roomName(std::string room) {
	roomName = room;
}

void Session::set_socket(SOCKET socket) {
	this->socket = socket;
}