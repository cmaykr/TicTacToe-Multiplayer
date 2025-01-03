#include <iostream>
#include <array>

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
    bool gameOver = false;
    bool playerOneTurn = true;

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