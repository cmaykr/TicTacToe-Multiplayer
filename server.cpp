#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

#include "serverSocket.hpp"
#include "game.hpp"

int serverFD;
int clientFD;

void createServer(int port)
{
    struct addrinfo hints;
    struct addrinfo *rp, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err = getaddrinfo(NULL, "8180", &hints, &result);
    if (err < 0)
    {
        std::cout << "Error getting address" << std::endl;
    }

    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        serverFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (serverFD == -1)
            continue;
        if (bind(serverFD, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            break;
        }

        close(serverFD);
    }

    freeaddrinfo(result);
    std::cout << serverFD << std::endl;
    listen(serverFD, 5);
}

int main(int argc, char** argv)
{
    bool gameOver = true;
    bool playerOneTurn = true;

    Game game{};

    createServer(8080);

    int clientFD = accept(serverFD, NULL, NULL);
    close(serverFD);

    if (clientFD == -1)
    {
        std::cout << "Fail" << std::endl;
    }

    //game.startGame();
    while (game.isOver() == false)
    {
        std::cout << game.getBoard() << std::endl;

        std::string board{game.getBoard()};
        if (game.isPlayerOnesTurn())
        {
            board.insert(board.end(), '1');
            send(clientFD, board.c_str(), board.size(), 0);
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            board.insert(board.end(), '2');
            send(clientFD, board.c_str(), board.size(), 0);
            std::cout << "Player two's turn, choose an empty spot to place your o. 1-9" << std::endl;
        }

        int spot{};
        if (game.isPlayerOnesTurn())
        {
            std::cin >> spot;
            while (!game.playMove(spot))
            {
                std::cin >> spot;
            }
        }
        else
        {
            char buf[1];
            int N = recv(clientFD, buf, sizeof(buf), 0);
            spot = buf[0];
            while (!game.playMove(spot))
            {
                send(clientFD, "Invalid", 7, 0);
                recv(clientFD, buf, sizeof(buf), 0);
                spot = buf[0];
            }
        }

        if (game.isOver())
        {
            std::cout << "Player " << ((game.isPlayerOnesTurn()) ? "one" : "two") << " won." << std::endl;
            std::cout << "Final board:" << std::endl;
            std::cout << game.getBoard() << std::endl;
            send(clientFD, game.getBoard().c_str(), game.getBoard().size(), 0);
        }
    }

    close(clientFD);
    
    return 0;
}