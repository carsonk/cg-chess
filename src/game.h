/*
    Copyright 2017, Nicholas Jankowski, Carson Killbreath, Nathan Oles,
    Richard Peterson, Rebecca Roughton, Benjamin Schnell

    This file is part of cg-chess.

    cg-chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cg-chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cg-chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define NUM_RANKS 8
#define NUM_FILES 8

typedef enum {
    PIECE_EMPTY = 0,
    PIECE_PAWN,
    PIECE_KNIGHT,
    PIECE_BISHOP,
    PIECE_ROOK,
    PIECE_QUEEN,
    PIECE_KING,
    PIECE_BPAWN,
    PIECE_BKNIGHT,
    PIECE_BBISHOP,
    PIECE_BROOK,
    PIECE_BQUEEN,
    PIECE_BKING
} GAME_PIECE;

typedef enum {
    COLOR_WHITE,
    COLOR_BLACK
} GAME_COLOR;

typedef struct {
    GAME_PIECE pieces[NUM_RANKS][NUM_FILES];
    unsigned move_num;
    GAME_COLOR current_turn;
} BOARD_STATE;

extern BOARD_STATE boardState;

bool Game_Init(void);
void Game_Logic(uint32_t currentTick);
void Game_Quit(void);
