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

/*
    Game Input Notes:

    2D Mode
        1. Calculate the Tile when a Click Occurs
            Matrix transformations will occur in reverse order, mapping a window
            coordinate to a world coordinate.
            This must be done twice, one mapping for the near-plane, and one
            mapping for the far-plane.
            These two points form a line.
            Using line/plane intersection, it will be determined what tile was
            clicked upon.
            In this mode, only the tile clicked on matters, not the piece.
            Each tile either has a piece occupying it, or it doesn't.
            Given the tile, the game logic will implicitly know the selected
            piece.
    3D Mode
        1. Calculate the Tile and Piece when a Click Occurs
            Matrix transformations will occur in reverse order, mapping a window
            coordinate to a world coordinate.
            This must be done twice, one mapping for the near-plane, and one
            mapping for the far-plane.
            These two points form a line.
            Using line/plane intersection, it will be determined what tile and
            what piece was clicked upon.
            The game logic will choose what to do given the selected tile and
            selected piece.
            e.x. If no piece is selected, the game will prioritize the newly
            clicked upon piece over the newly clicked upon tile.
            (User selecting a piece.)
            If a piece is already selected, the game will prioritize the newly
            clicked upon tile over the newly clicked upon piece.
            (User attempting to move a piece.)

    Picking may be implemented separately in utility functions.
    The renderer may need to transform chess piece bounding boxes into world
    coordinates, so as to facilitate picking.

*/


#include "input.h"


bool Input_Init(void)
{
    return true;
}


void Input_Logic(uint32_t currentTick)
{

}


void Input_Quit(void)
{

}
