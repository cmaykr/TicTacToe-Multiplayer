#include <iostream>
#include <string.h>
#include <array>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <charconv>
#include <unistd.h>

std::array<std::array<int, 3>, 3> gameBoard{};

void drawGameBoard()
{
    for (int i{}; i < 3; i++)
    {
        for (int j{}; j < 3; j++)
        {
            if (gameBoard[i][j] == 0)
            {
                std::cout << (i)*3+(j+1);
            }
            else
            {
                if (gameBoard[i][j] == 1)
                    std::cout << "x";
                else
                    std::cout << "o";
                
            }
            if (j != 2)
            {
                std::cout << "|";
            }

        }
        std::cout << std::endl;
    }
}

bool checkIfEnd()
{
    for (int i{}; i < 3; i++)
    {
        int sumHor{};
        int sumVert{};

        for (int j{}; j < 3; j++)
        {
            sumVert += gameBoard[i][j];
            sumHor += gameBoard[j][i];
        }

        if (abs(sumVert) == 3 || abs(sumHor) == 3)
        {
            return true;
        }
    }

    int sumDiagRight{};
    int sumDiagLeft{};
    sumDiagRight = gameBoard[0][0] + gameBoard[1][1] + gameBoard[2][2];
    sumDiagLeft = gameBoard[2][0] + gameBoard[1][1] + gameBoard[0][2];
    
    if (abs(sumDiagLeft) == 3 || abs(sumDiagRight) == 3)
        return true;

    return false;
}

int main(int argc, char** argv)
{
    bool gameOver = true;
    bool playerOneTurn = true;

    struct addrinfo hints;
    struct addrinfo *rp, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err = getaddrinfo(NULL, "8080", &hints, &result);
    if (err < 0)
    {
        std::cout << "Error getting address" << std::endl;
    }

    int serverSocket {0};
    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        serverSocket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (serverSocket == -1)
            continue;
        if (bind(serverSocket, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            char host[NI_MAXHOST], service[NI_MAXSERV];
            int s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            if (s == 0)
            {
                std::cout << host << " dd " << service << std::endl;
            }
            break;
        }
        else
        {
            std::cout << "Fail" << std::endl;
            std::cout << errno << std::endl;
        }

        //close(serverSocket);
    }

    freeaddrinfo(result);
    std::cout << serverSocket << std::endl;
    if (listen(serverSocket, 5) == EBADF)
    {
        std::cout << errno << std::endl;
    }

    std::cout << "Waiting to recieve" << std::endl;
    int client = accept(serverSocket, NULL, NULL);
    if (client == -1)
     {
        std::cout << "Client failed" << std::endl;
     }

    char buf[1024];
    recv(client, buf, sizeof(buf), 0);
    std::cout << "Message: " << buf << std::endl;

    close(client);
    close(serverSocket);
    while (gameOver == false)
    {
        std::cout << gameOver << std::endl;
        drawGameBoard();
        if (playerOneTurn)
        {
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            std::cout << "Player two's turn, choose an empty spot to place your o. 1-9" << std::endl;
        }

        bool run = true;
        while (run)
        {
            int spot{};
            std::cin >> spot;
            spot--;

            if (gameBoard[spot / 3][spot % 3] == 0 && spot >= 0 && spot <= 8)
            {
                gameBoard[spot / 3][spot % 3] = (playerOneTurn) ? 1 : -1;
                run = false;
            }
            else if (spot < 0 || spot > 8)
            {
                std::cout << "Incorrect value, choose a spot within 1-9" << std::endl;
            }  
            else
            {
                std::cout << "Spot already taken, choose another: ";
            }
        }

        if (checkIfEnd())
        {
            std::cout << "Player " << ((playerOneTurn) ? "one" : "two") << " won." << std::endl;
            std::cout << "Final board:" << std::endl;
            drawGameBoard();
            gameOver = true;
        }
        playerOneTurn = !playerOneTurn;
    }
    
    return 0;
}