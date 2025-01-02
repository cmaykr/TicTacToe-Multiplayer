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
                std::cout << (i+1)*(j+1);
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
        for (int j{}; j < 3; j++)
        {
            
        }
    }
}

int main(int argc, char** argv)
{
    bool gameOver = false;
    bool playerOneTurn = true;

    while (!gameOver)
    {
        drawGameBoard();
        if (playerOneTurn)
        {
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            std::cout << "Player two's turn, choose an empty spot to place your o. 1-9" << std::endl;
        }

        int spot{};
        std::cin >> spot;
        spot--;

        if (spot <= 3)
        {
            gameBoard[0][spot] = (playerOneTurn) ? 1 : 2;
        }
        else if (spot <= 5)
        {
            gameBoard[1][spot % 3] = (playerOneTurn) ? 1 : 2;
        }
        else
        {
            gameBoard[2][spot % 3] = (playerOneTurn) ? 1 : 2;
        }


        playerOneTurn = !playerOneTurn;
    }
    
    return 0;
}