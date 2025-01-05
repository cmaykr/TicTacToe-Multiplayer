#include <iostream>
#include <string.h>

#include "serverSocket.hpp"
#include "game.hpp"

int main(int argc, char** argv)
{
    bool gameOver = true;
    bool playerOneTurn = true;

    ServerSocket serv{};

    //serv.createServer(8080);

    //std::cout << serv.recieveMessage() << std::endl;

    //serv.closeServer();

    Game game{};

    game.startGame();
    // while (game.isOver() == false)
    // {
    //     std::cout << game.getBoard() << std::endl;
    //     if (game.isPlayerOnesTurn())
    //     {
    //         std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "Player two's turn, choose an empty spot to place your o. 1-9" << std::endl;
    //     }

    //     int spot{};
    //     std::cin >> spot;
    //     while (!game.playMove(spot))
    //     {
    //         std::cin >> spot;
    //     }

    //     if (game.isOver())
    //     {
    //         std::cout << "Player " << ((game.isPlayerOnesTurn()) ? "one" : "two") << " won." << std::endl;
    //         std::cout << "Final board:" << std::endl;
    //         std::cout << game.getBoard() << std::endl;
    //     }
    // }
    
    return 0;
}