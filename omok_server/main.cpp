#include <iostream>
#include "Server.h"

int main() {
    int port = 9190;

    try {
        Server server(port);
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
