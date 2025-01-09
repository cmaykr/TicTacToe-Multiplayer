#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <poll.h>

#include "game.hpp"
#include "socket.hpp"
#include <limits>

int serverFD;
int clientFD;

void printError()
{
    std::cout << "Error code: " << strerror(errno) << std::endl;
}

void createServer(int port)
{
    struct addrinfo hints;
    struct addrinfo *rp, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char buf[4];
    std::to_chars(buf, buf + sizeof(buf), port);
    int err = getaddrinfo(NULL, buf, &hints, &result);
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
    listen(serverFD, 5);
}

int main(int argc, char** argv)
{
    bool gameOver = false;
    bool playerOneTurn = true;

    Game game{};
    Socket server{false};
    server.initializeServer(std::stoi(argv[1]));
    Socket client = server.acceptConnection();

    //createServer(std::stoi(argv[1]));

    //int clientFD = accept(0, NULL, NULL);
    //close(serverFD);

    if (clientFD == -1)
    {
        std::cout << "Failed to connect to the client." << std::endl;
        printError();
    }

    while (game.isOver() == false)
    {
        std::cout << game.getBoard() << std::endl;

        std::string board{game.getBoard()};
        if (game.isPlayerOnesTurn())
        {
            board.insert(board.end(), '1');
            // if (send(clientFD, board.c_str(), board.size(), 0) == -1)
            // {
            //     printError();
            //     close(clientFD);
            //     return -1;
            // }
            client.sendMessage(board);
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            board.insert(board.end(), '2');
            // if (send(clientFD, board.c_str(), board.size(), 0) == -1)
            // {
            //     printError();
            //     close(clientFD);
            //     return -1;
            // }
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
            // char buf[1];
            // struct pollfd fds[1];
            // fds[0].fd = clientFD;
            // fds[0].events = POLLIN;
            // int N{};
            // int polls = poll(fds, 1, -1);
            // if (polls > 0)
            //     N = recv(clientFD, buf, sizeof(buf), 0);
            // else if (polls < 0)
            // {
            //     printError();
            //     close(clientFD);
            //     return -1;
            // }

            //spot = buf[0];
            spot = std::stoi(client.pollAndRecieveMessage());

            while (!game.playMove(spot))
            {
                //send(clientFD, "Invalid", 7, 0);
                // recv(clientFD, buf, sizeof(buf), 0);
                // spot = buf[0];

                client.sendMessage("invalid");
                spot = std::stoi(client.pollAndRecieveMessage());
            }
            

            send(clientFD, "valid", 5, 0);
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
            //send(clientFD, board.c_str(), board.size(), 0);
            client.sendMessage(board);
        }
    }

    //close(clientFD);
    
    return 0;
}