/**
 * Game Constants - Header File
 * libMorris
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 *
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

/** Number of pieces (per player) */
const unsigned char NUM_OF_PIECES = 9;

/** Number of field squares */
const unsigned char NUM_OF_SQUARES = 3;

/** Number of places in a field square */
const unsigned char NUM_OF_SQUARE_PLACES = 8;

/** Number of places in the field */
const unsigned char NUM_OF_FIELD_PLACES = NUM_OF_SQUARES * NUM_OF_SQUARE_PLACES;

/** Number of places to shift for next side starting point */
const unsigned char NUM_OF_PLACES_TO_SHIFT = NUM_OF_SQUARE_PLACES / 4;

/** Number of players */
const unsigned char NUM_OF_PLAYERS = 2;

/** Empty field place */
const unsigned char EMPTY_PLACE = 0;

#endif // GAME_CONSTANTS_H
