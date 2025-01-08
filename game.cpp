#include "game.hpp"

#include <sstream>

void Game::startGame()
{
    while (isOver() == false)
    {
        std::cout << getBoard() << std::endl;
        if (isPlayerOnesTurn())
        {
            std::cout << "Player one's turn, choose an empty spot to place your x. 1-9" << std::endl;
        }
        else
        {
            std::cout << "Player two's turn, choose an empty spot to place your o. 1-9" << std::endl;
        }

        int spot{};
        std::cin >> spot;
        while (!playMove(spot))
        {
            std::cin >> spot;
        }

        if (isOver())
        {
            std::cout << "Player " << ((isPlayerOnesTurn()) ? "one" : "two") << " won." << std::endl;
            std::cout << "Final board:" << std::endl;
            std::cout << getBoard() << std::endl;
        }
    }
}

bool Game::playMove(int spot)
{
    spot--;

    if (gameBoard[spot / 3][spot % 3] == 0 && spot >= 0 && spot <= 8)
    {
        gameBoard[spot / 3][spot % 3] = (isPlayerOne) ? 1 : -1;
        if (!checkIfEnd())
            isPlayerOne = !isPlayerOne;

        return true;
    }


    return false;
}

bool Game::isPlayerOnesTurn() const
{
    return isPlayerOne;
}

bool Game::isOver() const
{
    return gameOver;
}

std::string Game::getBoard() const
{
    std::string board{};
    std::stringstream boardStream{};

    for (int i{}; i < 3; i++)
    {
        for (int j{}; j < 3; j++)
        {
            if (gameBoard[i][j] == 0)
            {
                boardStream << (i)*3+(j+1);
            }
            else
            {
                if (gameBoard[i][j] == 1)
                    boardStream << "x";
                else
                    boardStream << "o";
                
            }
            if (j != 2)
            {
                boardStream << "|";
            }

        }
        boardStream << std::endl;
    }
    return boardStream.str();
}

void Game::drawGameBoardToOstream(std::ostream & out) const
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

bool Game::checkIfEnd()
{
    int emptySpots{};
    for (int i{}; i < 3; i++)
    {
        int sumHor{};
        int sumVert{};

        for (int j{}; j < 3; j++)
        {
            sumVert += gameBoard[i][j];
            sumHor += gameBoard[j][i];

            if (gameBoard[i][j] == 0)
                emptySpots++;
        }

        if (abs(sumVert) == 3 || abs(sumHor) == 3)
        {
            gameOver = true;
            return true;
        }
    }

    int sumDiagRight{};
    int sumDiagLeft{};
    sumDiagRight = gameBoard[0][0] + gameBoard[1][1] + gameBoard[2][2];
    sumDiagLeft = gameBoard[2][0] + gameBoard[1][1] + gameBoard[0][2];
    
    if (abs(sumDiagLeft) == 3 || abs(sumDiagRight) == 3)
    {
        gameOver = true;
        return true;
    }

    if (emptySpots == 0)
    {
        isDraw = true;
        gameOver = true;
        return true;
    }
    return false;
}

int Game::getWinner() const
{
    if (!isOver())
        return -1;

    if (isDraw)
        return 0;

    if (isPlayerOnesTurn())
        return 1;

    return 2;
}