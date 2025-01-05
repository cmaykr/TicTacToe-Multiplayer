#pragma once

#include <string>

class Connection
{
public:
    Connection() = default;

    void connectToServer(std::string const& address, int port);

    void closeConnection() const;

    void sendMessage(std::string const& message);

private:
    int socketFD{};
};