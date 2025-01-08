#pragma once

#include <array>
#include <iostream>

class Game
{
public:
    Game() = default;

    /// @brief Starts the game loop and everything with the game.
    void startGame();


    /// @brief Places either an x or 0 on the chosen spot, depending on the current player that is placing.
    /// @param spot The spot 1-9 to place the mark on.
    /// @return True if the play succeeded or false if the spot was taken or out of bounds.
    bool playMove(int spot);
    /// @brief Checks which player's turn it is.
    /// @return True if it's the first players turn, false if it's the second players turn.
    bool isPlayerOnesTurn() const;
    /// @brief Checks if the game is over, i.e one player has three in a row.
    /// @return True if the game is over, false otherwise.
    bool isOver() const;

    /// @brief Gets the board in a string format.
    /// @return The board.
    std::string getBoard() const;
private:
    void drawGameBoardToOstream(std::ostream & out) const;
    bool checkIfEnd();

    std::array<std::array<int, 3>, 3> gameBoard{};

    bool gameOver{false};
    bool isPlayerOne{true};
};