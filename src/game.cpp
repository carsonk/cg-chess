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
#include "render.h"
#include "SDL_log.h"

Position currentPosition;
StateListPtr States(new std::deque<StateInfo>(1));

// Opening game state.
const char *startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BOARD_STATE boardState = {};

GAME_STATUS gameStatus = GSTATUS_NOCHANGE;

BOARD_STATE getEmptyBoard(void);
void refreshBoardState(Position *newPosition);

Square fromSquare = SQ_NONE;

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

    States = StateListPtr(new std::deque<StateInfo>(1));
    currentPosition.set(startFEN, false, &States->back(), Threads.main());

    boardState = getEmptyBoard();

    return true;
}


void Game_Logic(uint32_t currentTick)
{
    Rank rank = RANK_NB;
    File file = FILE_NB;
    Square square = SQ_NONE;
    Move moveAttempt = MOVE_NONE;
    StateInfo st;

    if (userClickedTileLastFrame) 
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clicked rank/file: %d / %d", lastFrameClickedRank, lastFrameClickedFile);
        
        rank = (Rank)lastFrameClickedRank;
        file = (File)lastFrameClickedFile;
        square = make_square(file, rank);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Stockfish tile no: %d", square);

        if (fromSquare == square && fromSquare != SQ_NONE)
        {
            // If the square is clicked again, clear it.
            fromSquare = SQ_NONE;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Source square was clicked. Unselecting...", square);
        }
        else if (fromSquare != SQ_NONE)
        {
            // If a different target square is clicked, build a move attempt.
            moveAttempt = make_move(fromSquare, square);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Constructing move attempt.", square);
            fromSquare = SQ_NONE;
        }
        else
        {
            fromSquare = square;
        }
    }

    if (moveAttempt != MOVE_NONE && moveAttempt != MOVE_NULL) 
    {
        if (currentPosition.legal(moveAttempt)) {
            // Move is legal. Update board state, change move, etc.

            gameStatus = GSTATUS_MOVE_SUCCESS;

            if (currentPosition.gives_check(moveAttempt))
                gameStatus = GSTATUS_MOVE_SUCCESS_CHECK;

            currentPosition.do_move(moveAttempt, st);

            // Refresh our board state.
            refreshBoardState(&currentPosition);

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Move performed.", square);
        } else {
            // Move is not legal.

            gameStatus = GSTATUS_MOVE_INVALID;

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Illegal move!", square);
        }
    } else {
        gameStatus = GSTATUS_NOCHANGE;
    }
}


void Game_Quit(void)
{

}

BOARD_STATE getEmptyBoard(void) {
    BOARD_STATE freshBoard;
    for (int i = 0; i < NUM_RANKS; i++) {
        for (int j = 0; j < NUM_FILES; j++) {//Board is being filled from Bottom left(white), along the file.
            if (i == 0) { //Rank 1
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
            else if (i == 1) {//Rank 1 -- Pawns
                freshBoard.pieces[i][j] = PIECE_PAWN;
            }
            else if (i == 6) {//Rank 6 -- BPawns
                freshBoard.pieces[i][j] = PIECE_BPAWN;
            }
            else if (i == 7) { //Rank 7
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

void refreshBoardState(Position *newPosition) {
    for (int rank = 0; rank < NUM_RANKS; rank++) {
        for (int file = 0; file < NUM_FILES; file++) {
            GAME_PIECE piece = PIECE_EMPTY;
            switch (newPosition->piece_on(make_square((File)file, (Rank)rank))) {
            case NO_PIECE:
                piece = PIECE_EMPTY;
                break;
            case W_PAWN:
                piece = PIECE_PAWN;
                break;
            case W_KNIGHT:
                piece = PIECE_KNIGHT;
                break;
            case W_BISHOP:
                piece = PIECE_BISHOP;
                break;
            case W_ROOK:
                piece = PIECE_ROOK;
                break;
            case W_QUEEN:
                piece = PIECE_QUEEN;
                break;
            case W_KING:
                piece = PIECE_KING;
                break;
            case B_PAWN:
                piece = PIECE_BPAWN;
                break;
            case B_KNIGHT:
                piece = PIECE_BKNIGHT;
                break;
            case B_BISHOP:
                piece = PIECE_BBISHOP;
                break;
            case B_ROOK:
                piece = PIECE_BROOK;
                break;
            case B_QUEEN:
                piece = PIECE_BQUEEN;
                break;
            case B_KING:
                piece = PIECE_BKING;
                break;
            }

            boardState.pieces[rank][file] = piece;
        }
    }
}
