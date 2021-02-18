/**
 * Game Class
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Game.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

// TODO: REWORK LOGGING
// #include "../eMorrisGUI/_Source/engine/UtilityFunctions.hpp"

// TODO: REMOVE
std::array<unsigned char, NUM_OF_FIELD_PLACES>* Game::GetField()
{
    return &field;
}

Game::Game()
{
    // Initialize
    for (unsigned char index = 0; index < NUM_OF_PLAYERS; index++)
    {
        deck[index] = NUM_OF_PIECES;
        numOfPieces[index] = 0;
    }

    // Select starting player
    srand(time(nullptr));
    currentPlayer = rand() % NUM_OF_PLAYERS;

    // Setting game state
    state = GameState::Place;
}

GameState Game::GetGameState()
{
    return state;
}

inline unsigned char Game::GetCurrentPlayer()
{
    return currentPlayer + 1;
}

unsigned char Game::GetDeck()
{
    return deck[currentPlayer];
}

unsigned char Game::GetNumberOfPieces()
{
    return numOfPieces[currentPlayer];
}

unsigned char Game::GetNumberOfMills()
{
    return numOfMills;
}

void Game::CheckState()
{
    switch (state)
    {
    case GameState::Init:
        break;

    case GameState::Place:
    case GameState::Move:
        NextPlayer();

        // Set game state to end if the opponent has not enough pieces
        if (numOfPieces[currentPlayer] < 3 && deck[currentPlayer] == 0)
        {
            state = GameState::End;
            NextPlayer();
            break;
        }
        NextPlayer();

        // Set game state to remove if the current player has a mill
        if (CheckForMills())
        {
            state = GameState::Remove;
            break;
        }
        NextPlayer();

        // Set game state to move if the opponent's deck is empty
        if (deck[currentPlayer] == 0)
        {
            // Set game state to end if the opponent can't move
            if (!CheckHasMove())
            {
                state = GameState::End;
                NextPlayer();
                break;
            }

            state = GameState::Move;
            break;
        }

        break;

    case GameState::Remove:
        NextPlayer();
        // Set game state to end if the opponent has not enough pieces
        if (numOfPieces[currentPlayer] < 3 && deck[currentPlayer] == 0)
        {
            state = GameState::End;
            NextPlayer();
            break;
        }
        NextPlayer();

        // Stay in this state if the current player still has mills
        if (--numOfMills > 0)
        {
            break;
        }
        NextPlayer();

        // Set game state to place if the opponent's deck is not empty
        if (deck[currentPlayer] > 0)
        {
            CheckForMills();
            state = GameState::Place;
            break;
        }

        // Set game state to end if the opponent can't move
        if (!CheckHasMove())
        {
            state = GameState::End;
            NextPlayer();
            break;
        }

        // Set game state to move if the piece was removed
        state = GameState::Move;
        break;

    case GameState::End:
        break;
    }
}

bool Game::Place(unsigned char point)
{
    if (state != GameState::Place || !CheckPlace(point))
    {
        return false;
    }

    // Set field point to current player's index
    field[point] = GetCurrentPlayer();

    deck[currentPlayer]--;
    numOfPieces[currentPlayer]++;

    return true;
}

bool Game::Move(unsigned char fromPoint, unsigned char toPoint)
{
    // Check first part of the move
    if (toPoint == 255)
    {
        if (state != GameState::Move || !CheckRemove(fromPoint, true))
        {
            return false;
        }
    }
    // Check both parts of the move
    else
    {
        if (state != GameState::Move || !CheckMove(fromPoint, toPoint))
        {
            return false;
        }

        // Set field point "to" to current player and field point "from" to empty
        field[toPoint] = field[fromPoint];
        field[fromPoint] = EMPTY_PLACE;
    }

    return true;
}

bool Game::Remove(unsigned char point)
{
    if (state != GameState::Remove || !CheckRemove(point))
    {
        return false;
    }

    // Set field point to empty
    field[point] = EMPTY_PLACE;

    NextPlayer();
    numOfPieces[currentPlayer]--;
    NextPlayer();

    return true;
}

void Game::NextPlayer()
{
    // Set the next player as the current player
    currentPlayer++;
    if (currentPlayer >= NUM_OF_PLAYERS)
    {
        currentPlayer = 0;
    }
}

bool Game::CheckHasMove()
{
    if (numOfPieces[currentPlayer] <= 3)
    {
        return true;
    }

    for (unsigned char place = 0; place < NUM_OF_FIELD_PLACES; ++place)
    {
        if (field[place] != GetCurrentPlayer())
        {
            continue;
        }

        std::vector<unsigned char>::const_iterator api;
        for (api = adjacentPlaces[place].cbegin(); api != adjacentPlaces[place].cend(); ++api)
        {
            if (field[*api] == EMPTY_PLACE)
            {
                return true;
            }
        }
    }

    return false;
}

bool Game::CheckPlace(unsigned char point)
{
    // Check if point is empty
    if (field[point] != 0)
    {
        return false;
    }

    return true;
}

bool Game::CheckMove(unsigned char fromPoint, unsigned char toPoint)
{
    // Check if from point is the same as to point
    if (fromPoint == toPoint)
    {
        return false;
    }

    // Check if from point has the current player's piece
    if (field[fromPoint] != GetCurrentPlayer())
    {
        return false;
    }

    // Check if to point is empty
    if (field[toPoint] != EMPTY_PLACE)
    {
        return false;
    }

    // Check move if the player can not jump
    if (numOfPieces[currentPlayer] > 3)
    {
        // Check if player moves to the adjacent point (and does not jump to next square starting point)
        if ((toPoint != fromPoint - 1 && toPoint != fromPoint + 1) || (fromPoint % NUM_OF_SQUARE_PLACES == 0
                && toPoint == fromPoint - 1) || (fromPoint % NUM_OF_SQUARE_PLACES == NUM_OF_SQUARE_PLACES - 1
                        && toPoint == fromPoint + 1))
        {
            // Check if player moves where the square is closing (abs(from-to)==7)
            if ((fromPoint % NUM_OF_SQUARE_PLACES != 0 || toPoint != fromPoint + NUM_OF_SQUARE_PLACES - 1)
                    && (fromPoint % NUM_OF_SQUARE_PLACES != NUM_OF_SQUARE_PLACES - 1 || toPoint != fromPoint - NUM_OF_SQUARE_PLACES + 1))
            {
                // Check if player moves to another square
                if (fromPoint % 2 != 1 || (toPoint != fromPoint - NUM_OF_SQUARE_PLACES && toPoint != fromPoint + NUM_OF_SQUARE_PLACES))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Game::CheckRemove(unsigned char place, bool currentPlayerCheck)
{
    // Check if place is empty
    if (field[place] == 0)
    {
        return false;
    }
    // Check if place has the current player's piece
    else if (currentPlayerCheck && field[place] != GetCurrentPlayer())
    {
        return false;
    }
    // Check if place has the opponent's piece
    else if (!currentPlayerCheck)
    {
        if (field[place] == GetCurrentPlayer())
        {
            return false;
        }

        // Check if piece is included in a mill and has other pieces that are not
        std::array<bool, NUM_OF_FIELD_PLACES> isMills = { false };
        NextPlayer(); // TODO: replace with more elegant solution
        CheckForMills(&isMills);

        if (isMills[place])
        {
            unsigned char count = 0;
            for (unsigned char index = 0; index < NUM_OF_FIELD_PLACES; ++index)
            {
                if (isMills[index])
                {
                    ++count;
                }
            }
            if (count != numOfPieces[currentPlayer])
            {
                NextPlayer(); // TODO: replace with more elegant solution
                return false;
            }
        }
        NextPlayer(); // TODO: replace with more elegant solution
    }

    return true;
}

bool Game::CheckForMills(std::array<bool, NUM_OF_FIELD_PLACES>* isMills)
{
    std::vector<unsigned char> mills;

    // Loop through the squares
    for (unsigned char square = 0; square < NUM_OF_SQUARES; ++square)
    {
        // Loop through the sides of a square
        for (unsigned char place = square * NUM_OF_SQUARE_PLACES;
                place < (square + 1) * NUM_OF_SQUARE_PLACES; place += NUM_OF_PLACES_TO_SHIFT)
        {
            if (CheckMill(place, isMills))
            {
                mills.push_back(place);
            }
        }
    }

    // Loop through the connections of the squares
    for (unsigned char place = 1; place < NUM_OF_SQUARE_PLACES; place += NUM_OF_PLACES_TO_SHIFT)
    {
        if (CheckMill(place, isMills))
        {
            mills.push_back(place);
        }
    }

    // Don't update
    if (isMills != nullptr)
    {
        return false;
    }

    // Looking for new mills
    numOfMills = 0;
    for (std::vector<unsigned char>::iterator it = mills.begin(); it != mills.end();
            ++it)
    {
        std::vector<unsigned char>::iterator itFind = std::find(
                    this->mills[currentPlayer].begin(), this->mills[currentPlayer].end(), *it);

        if (itFind == this->mills[currentPlayer].end())
        {
            numOfMills++;
        }
    }

    // Replace stored mills with the current ones
    this->mills[currentPlayer].clear();
    this->mills[currentPlayer].reserve(mills.size());
    for (std::vector<unsigned char>::iterator it = mills.begin(); it != mills.end(); ++it)
    {
        this->mills[currentPlayer].push_back(*it);
    }

    // Return true if the current player has a mill
    if (numOfMills > 0)
    {
        return true;
    }

    return false;
}

bool Game::CheckMill(unsigned char place, std::array<bool, NUM_OF_FIELD_PLACES>* isMills)
{
    std::array<unsigned char,  NUM_OF_PLACES_TO_SHIFT + 1> places = GetMillPlaces(place);
    if (field[places[0]] == GetCurrentPlayer() && field[places[1]] == field[places[0]]
            && field[places[2]] == field[places[0]])
    {
        if (isMills != nullptr)
        {
            isMills->at(places[0]) = true;
            isMills->at(places[1]) = true;
            isMills->at(places[2]) = true;
        }
        return true;
    }

    return false;
}

std::array<unsigned char, NUM_OF_PLACES_TO_SHIFT + 1> Game::GetMillPlaces(unsigned char place)
{
    std::array<unsigned char,  NUM_OF_PLACES_TO_SHIFT + 1> places;

    // Get mill places on the connection of the squares
    if (place % 2 == 1 && place < NUM_OF_SQUARE_PLACES)
    {
        places = { place, place + NUM_OF_SQUARE_PLACES, place + NUM_OF_SQUARE_PLACES * 2 };
    }

    // Get mill places on the side of a square
    if (place % 2 == 0 && place < NUM_OF_FIELD_PLACES)
    {
        unsigned char placePlusTwo = (place + 2) % NUM_OF_SQUARE_PLACES == 0 ? (place / NUM_OF_SQUARE_PLACES) *
                                     NUM_OF_SQUARE_PLACES : place + 2;
        places = { place, place + 1, placePlusTwo };
    }

    return places;
}
