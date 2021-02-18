/**
 * Game Class - Header File
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef GAME_H
#define GAME_H

#include <array>
#include <unordered_map>
#include <vector>

#include "GameConstants.hpp"
#include "GameState.hpp"

class Game
{
private:

    /** @brief Field
     * has 24 points, indexed like this:
     *
     * 0-----------1-----------2
     * |           |           |
     * |   8-------9------10   |
     * |   |       |       |   |
     * |   |  16--17--18   |   |
     * |   |   |       |   |   |
     * 7--15--23      19--11---3
     * |   |   |       |   |   |
     * |   |  22--21--20   |   |
     * |   |       |       |   |
     * |  14------13------12   |
     * |           |           |
     * 6-----------5-----------4
     *
     * the state of the fields can be:
     * 0 - empty
     * 1 - player 1
     * 2 - player 2
     */
    std::array<unsigned char, NUM_OF_FIELD_PLACES> field = { 0 };

    /** State of the games */
    GameState state = GameState::Init;

    /** Index of the current player */
    unsigned char currentPlayer;

    /** Deck of the players (number of pieces not placed yet) */
    unsigned char deck[NUM_OF_PLAYERS];

    /** Number of pieces of the players */
    unsigned char numOfPieces[NUM_OF_PLAYERS];

    /** Number of mills of the current player */
    unsigned char numOfMills;

    /** Mills of the players */
    std::array<std::vector<unsigned char>, NUM_OF_PLAYERS> mills;

    /** Adjacent field places */
    const std::array<std::vector<unsigned char>, NUM_OF_FIELD_PLACES> adjacentPlaces =
    {{
        { 1, 7 },
        { 0, 2, 9 },
        { 1, 3 },
        { 2, 4, 11 },
        { 3, 5 },
        { 4, 6, 13 },
        { 5, 7 },
        { 0, 6, 15 },
        { 9, 15 },
        { 1, 8, 10, 17 },
        { 9, 11 },
        { 3, 10, 12, 19 },
        { 11, 13 },
        { 5, 12, 14, 21 },
        { 13, 15 },
        { 7, 8, 14, 23 },
        { 17, 23 },
        { 9, 16, 18 },
        { 17, 19 },
        { 11, 18, 20 },
        { 19, 21 },
        { 13, 20, 22 },
        { 21, 23 },
        { 15, 16, 22 }
    }};

    /**
     * Advance to next player
     */
    void NextPlayer();

    /**
     * Check if the current player has move
     *
     * @return The current player has move
     */
    bool CheckHasMove();

    /**
     * Check placement of piece
     *
     * @param The point to place to
     * @param The point value to check for (default is 0)
     *
     * @return The placement of the piece is valid
     */
    bool CheckPlace(unsigned char point);

    /**
     * Check movement of piece
     *
     * @param The point to move from
     * @param The point to move to
     *
     * @return The movement of the piece is valid
     */
    bool CheckMove(unsigned char fromPoint, unsigned char toPoint = 255);

    /**
     * Check removal of piece
     *
     * @param[in] point The point to remove from
     * @param[in] currentPlayerCheck Check if current player's piece is at the point
     *
     * @return The removal of the piece is valid
     */
    bool CheckRemove(unsigned char point, bool currentPlayerCheck = false);

    /**
     * Check for mills
     *
     * @param[in,out] isMills Pointer to vector to store places for mills
     *
     * @return The current player has a mill
     */
    bool CheckForMills(std::array<bool, NUM_OF_FIELD_PLACES>* isMills = nullptr);

    /**
     * Check mill
     *
     * @param[in] place The beginning place of the mill
     * @param[in,out] isMills Pointer to vector to store places for mills
     *
     * @return The given places has a mill
     */
    bool CheckMill(unsigned char mill, std::array<bool, NUM_OF_FIELD_PLACES>* isMills = nullptr);

    // TODO: IMPLEMENT
//     /**
//      * Count pieces
//      */
//     void CountPieces();

    /**
     * @brief Get mill places
     *
     * Get the places where a mill could be
     * based on the place paramter given.
     *
     * @param[in] place The beginning place of the mill
     *
     * @return The array of the places
     */
    std::array<unsigned char, 3> GetMillPlaces(unsigned char place);

public:

    // TODO: REMOVE
    std::array<unsigned char, NUM_OF_FIELD_PLACES>* GetField();

    /**
     * Consruct game
     */
    Game();

    /**
     * Get game state
     *
     * @return The game state
     */
    GameState GetGameState();

    /**
     * Get current player
     *
     * @return The current player
     */
    inline unsigned char GetCurrentPlayer();

    /**
     * Get deck of the current player
     *
     * @return The number of pieces in the deck of the current player
     */
    unsigned char GetDeck();

    /**
     * Get number of pieces of the current player
     *
     * @return The number of pieces of the current player on the board
     */
    unsigned char GetNumberOfPieces();

    /**
     * Get number of mills of the current player
     *
     * @return The number of mills the current player has
     */
    unsigned char GetNumberOfMills();

    /**
     * Check the game state
     */
    void CheckState();

    /**
    * Place the piece
    *
    * @param The point to place to
    *
    * @return The placement of the piece is done
    */
    bool Place(unsigned char point);

    /**
     * Move the piece
     *
     * @param The point to move from
     * @param The point to move to (default is -1 to check first part of the move)
     *
     * @return The movement of the piece is done
     */
    bool Move(unsigned char fromPoint, unsigned char toPoint = -1);

    /**
     * Remove the piece
     *
     * @param The point to remove from
     *
     * @return The removal of the piece is done
     */
    bool Remove(unsigned char point);
};

#endif // GAME_H
