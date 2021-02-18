/**
 * Game State - Header File
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

/** State of the game */
enum GameState
{
    /** Initialization */
    Init,

    /** Wait for player to place a piece */
    Place,

    /** Wait for player to remove a piece */
    Remove,

    /** Wait for player to move a piece */
    Move,

    /** Game ended */
    End
};

#endif // GAME_STATE_H
