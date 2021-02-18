/**
 * Game Step Element - Header File
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef GAME_STEP_ELEMENT_H
#define GAME_STEP_ELEMENT_H

#include <array>

struct GameStepElement
{
    /** State of the game field */
    unsigned short state0 = 0;
    unsigned short state1 = 0;
    unsigned short state2 = 0;

    /** Changes - [0] remove from, [1] place to */
    unsigned char changes0 = 255;
    unsigned char changes1 = 255;

    /** Wins with this state */
    unsigned short wins = 0;

    /** Losses with this state */
    unsigned short losses = 0;

    /** Balance of the wins and losses */
    long balance = 0;
};

#endif // GAME_STEP_ELEMENT_H
