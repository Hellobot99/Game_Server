#include "Server.h"

Server::Server(int port) : port(port), server_fd(-1), epfd(-1)
{
    // Constructor implementation
}

Server::~Server()
{
    // Destructor implementation
    stop();
}

void Server::start()
{

}

void Server::stop()
{

}