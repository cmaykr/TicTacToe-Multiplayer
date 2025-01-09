#include <string>
#include <iostream>
#include <algorithm>
#include <limits>

#include "socket.hpp"

int main(int argc, char *argv[])
{
    int port{};
    std::string address{};
    bool debugOutput{};

    if (argc != 4)
    {
        std::cout << "Wrong amount of arguments";
        std::cout << "Arguments in order: [HOSTNAME] [PORT] [DEBUG]" << std::endl;

        return -1;
    }
    port = std::stoi(argv[2]);
    if (port < 1024 || port > 49151)
    {
        std::cerr << "Port number invalid, choose a port number between 1024-49151." << std::endl;
        return -1;
    }

    if (std::string(argv[3]) == "true")
    {
        debugOutput = true;
    }
    else if (std::string(argv[3]) == "false")
    {
        debugOutput = false;
    }
    else
    {
        std::cerr << "Debug output argument invalid, valid arguments are: true false" << std::endl;
        return -1;
    }

    address = argv[1]; // TODO: Check for correct format.


    Socket socket{debugOutput};
    socket.initializeClientAndConnectToServer(address, port);

    bool gameOver = false;
    while (gameOver == false)
    {
        std::string rec = socket.pollAndRecieveMessage();

        std::for_each(rec.begin(), rec.end() - 1, [](char c)
        {
            std::cout << c;
        });
        std::cout << std::endl;
        std::cout << *(--rec.end()) << std::endl;

        if (*(--rec.end()) == '2')
        {
            std::cout << "Your turn, choose an empty spot to place your o. 1-9" << std::endl;
            int spot{};
            std::cin >> spot;
            char message[1];
            message[0] = '0' + spot;
            std::cout << "Sending message: " << message[0] << " from: " << spot << std::endl;

            socket.sendMessage(message);
            std::string ans {socket.pollAndRecieveMessage()};
            while (ans == "invalid" && ans != "valid")
            {
                std::cout << "Mark can't be placed on that spot, choose another empty spot." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> spot;

                socket.sendMessage(std::to_string(spot));
                ans = socket.pollAndRecieveMessage();
            }
        }
        else if (*(--rec.end()) == '3')
        {
            std::cout << "Game is over, player one won the game." << std::endl;
            gameOver = true;
        }
        else if (*(--rec.end()) == '4')
        {
            std::cout << "Game is over, player two (you) won the game." << std::endl;
            gameOver = true;
        }
        else if (*(--rec.end()) == '1')
        {
            std::cout << "Other players turn" << std::endl;
        }
        else if (*(--rec.end()) == '5')
        {
            std::cout << "Game is a draw, tough luck!" << std::endl;
            gameOver = true;
        }
        else
        {
            std::cout << "Invalid message, ending session." << std::endl;
            gameOver = true;
        }
    }

    return 0;
}
