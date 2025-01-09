#include <string>
#include <iostream>

#include "game.hpp"
#include "socket.hpp"
#include <limits>

int main(int argc, char** argv)
{
    bool gameOver = false;
    bool playerOneTurn = true;
    bool debugOutput{};
    int port{};

    if (argc != 3)
    {
        std::cerr << "Invalid amount of parameters, need two parameters in order: [PORT] [DEBUG]" << std::endl;
        return -1;
    }
    port = std::stoi(argv[1]);
    if (port < 1024 || port > 49151)
    {
        std::cerr << "Port number invalid, choose a port number between 1024-49151." << std::endl;
        return -1;
    }

    if (std::string(argv[2]) == "true")
    {
        debugOutput = true;
    }
    else if (std::string(argv[2]) == "false")
    {
        debugOutput = false;
    }
    else
    {
        std::cerr << "Debug output argument invalid, valid arguments are: true false" << std::endl;
        return -1;
    }

    Game game{};
    Socket server{debugOutput};
    server.initializeServer(std::stoi(argv[1]));
    Socket client = server.acceptConnection();

    while (game.isOver() == false)
    {
        std::cout << game.getBoard() << std::endl;

        std::string board{game.getBoard()};
        if (game.isPlayerOnesTurn())
        {
            board.insert(board.end(), '1');

            client.sendMessage(board);
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            board.insert(board.end(), '2');

            client.sendMessage(board);
            std::cout << "Player two's turn, wait for them to place their mark. " << std::endl;
        }

        int spot{};
        if (game.isPlayerOnesTurn())
        {
            std::cin >> spot;
            while (!game.playMove(spot))
            {
                std::cout << "Mark can't be placed on that spot, choose another empty spot." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> spot;
            }
        }
        else
        {
            spot = std::stoi(client.pollAndRecieveMessage());

            while (!game.playMove(spot))
            {

                client.sendMessage("invalid");
                spot = std::stoi(client.pollAndRecieveMessage());
            }
            
            client.sendMessage("valid");
        }

        if (game.isOver())
        {
            int flag {game.getWinner()};
            
            board = game.getBoard();
            if (flag == 1)
            {
                std::cout << "Player one(you) won, congratulations!";
                board.insert(board.end(), '3');
            }
            else if (flag == 2)
            {
                std::cout << "Player two won, better luck next time!";
                board.insert(board.end(), '4');
            }
            else if (flag == 0)
            {
                std::cout << "The game is a draw, tough luck!";
                board.insert(board.end(), '5');
            }
            std::cout << std::endl;
            std::cout << "Final board:" << std::endl;
            std::cout << game.getBoard() << std::endl;

            client.sendMessage(board);
        }
    }
    
    return 0;
}