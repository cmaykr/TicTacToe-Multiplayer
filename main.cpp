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
    for (int i{}; i < 6; i++)
    {
        if (i < 3)
        {
            if (gameBoard[i][0] == gameBoard[i][1] && gameBoard[i][2] == gameBoard[i][1] && gameBoard[i][0] != 0 && gameBoard[i][1] != 0 && gameBoard[i][2] != 0)
            {
                return true;
            }
        }
        else if (i < 6)
        {
            if (gameBoard[0][i] == gameBoard[1][i] && gameBoard[2][i] == gameBoard[1][i] && gameBoard[0][i] != 0 && gameBoard[1][i] != 0 && gameBoard[2][i] != 0)
            {
                return true;
            }
        }
    }

    if (gameBoard[1][1] == gameBoard[0][0] && gameBoard[2][2] == gameBoard[1][1] && gameBoard[1][1] != 0 && gameBoard[2][2] != 0 && gameBoard[0][0] != 0)
    {
        return true;
    }
    
    if (gameBoard[1][1] == gameBoard[0][2] && gameBoard[2][0] == gameBoard[1][1] && gameBoard[1][1] != 0 && gameBoard[0][2] != 0 && gameBoard[2][0] != 0)
    {
        return true;
    }

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
                gameBoard[spot / 3][spot % 3] = (playerOneTurn) ? 1 : 2;
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