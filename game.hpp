#pragma once

#include <array>
#include <iostream>

class Game
{
public:
    Game() = default;

    /// @brief Starts the game loop and everything with the game.
    void startGame();


    bool playMove(int spot);
    bool isPlayerOnesTurn() const;
    bool isOver() const;

    std::string getBoard() const;
private:
    void drawGameBoardToOstream(std::ostream & out) const;
    bool checkIfEnd();

    std::array<std::array<int, 3>, 3> gameBoard{};

    bool gameOver{false};
    bool isPlayerOne{true};
};