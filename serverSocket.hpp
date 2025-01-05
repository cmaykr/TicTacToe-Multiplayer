#pragma once

#include <string>

class ServerSocket
{
public:
    ServerSocket() = default;

    void createServer(int port);

    std::string recieveMessage();

    void closeServer() const;
private:
    int socketFD{};
    int clientFD{};
};