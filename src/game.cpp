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

#include "game.h"

#include "Stockfish\src\bitboard.h"
#include "Stockfish\src\position.h"
#include "Stockfish\src\search.h"
#include "Stockfish\src\thread.h"
#include "Stockfish\src\tt.h"
#include "Stockfish\src\uci.h"
#include "Stockfish\src\syzygy\tbprobe.h"

Position currentPosition;
StateListPtr States(new std::deque<StateInfo>(1));

// Opening game state.
const char *startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

bool Game_Init(void)
{
    UCI::init(Options);
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Search::init();
    Pawns::init();
    Threads.init();
    Tablebases::init(Options["SyzygyPath"]);

    currentPosition.set(startFEN, false, &States->back(), Threads.main());

    return true;
}


void Game_Logic(uint32_t currentTick)
{
    Move moveAttempt;
}


void Game_Quit(void)
{

}

BOARD_STATE getEmptyBoard(void) {
    BOARD_STATE freshBoard;
    for (int i = 0; i < NUM_RANKS; i++) {
        for (int j = 0; j < NUM_FILES; j++) {//Board is being filled from Bottom left(white), along the file.
            if (i = 0) { //Rank 1
                switch (j) {
                case 0:
                case 7:
                    freshBoard.pieces[i][j] = PIECE_ROOK;
                    break;
                case 1:
                case 6:
                    freshBoard.pieces[i][j] = PIECE_KNIGHT;
                    break;
                case 2:
                case 5:
                    freshBoard.pieces[i][j] = PIECE_BISHOP;
                    break;
                case 3:
                    freshBoard.pieces[i][j] = PIECE_QUEEN;
                    break;
                case 4:
                    freshBoard.pieces[i][j] = PIECE_KING;
                    break;
                }
            }
            else if (i = 1) {//Rank 1 -- Pawns
                freshBoard.pieces[i][j] = PIECE_PAWN;
            }
            else if (i = 6) {//Rank 6 -- BPawns
                freshBoard.pieces[i][j] = PIECE_BPAWN;
            }
            else if (i = 7) { //Rank 7
                switch (j) {
                case 0:
                case 7:
                    freshBoard.pieces[i][j] = PIECE_BROOK;
                    break;
                case 1:
                case 6:
                    freshBoard.pieces[i][j] = PIECE_BKNIGHT;
                    break;
                case 2:
                case 5:
                    freshBoard.pieces[i][j] = PIECE_BBISHOP;
                    break;
                case 3:
                    freshBoard.pieces[i][j] = PIECE_BQUEEN;
                    break;
                case 4:
                    freshBoard.pieces[i][j] = PIECE_BKING;
                    break;
                }
            }
            else {
                freshBoard.pieces[i][j] = PIECE_EMPTY;
            }
        }
    }

    return freshBoard;
}
