#include <string.h>
#include <iostream>

#include "conn.hpp"

int main(int argc, char *argv[])
{
    int client{};
    int port{};
    std::string address{};

    if (argc != 3)
    {
        std::cout << "Wrong amount of arguments";
        std::cout << "Arguments in order: [HOSTNAME] [PORT]" << std::endl;

        return -1;
    }
    
    address = argv[1]; // TODO: Check for correct format.
    port = std::stoi(argv[2]); // TODO: Check for correct format and port number.


    Connection conn{};

    conn.connectToServer(address, port);

    conn.sendMessage("Test");

    conn.closeConnection();
    return 0;
}
