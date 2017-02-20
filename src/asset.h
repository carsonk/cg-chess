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

#include "nanosvg.h"
#include "tiny_obj_loader.h"

extern struct NSVGimage *svgPawnDark;
extern struct NSVGimage *svgRookDark;
extern struct NSVGimage *svgKnightDark;
extern struct NSVGimage *svgBishopDark;
extern struct NSVGimage *svgQueenDark;
extern struct NSVGimage *svgKingDark;

extern struct NSVGimage *svgPawnLight;
extern struct NSVGimage *svgRookLight;
extern struct NSVGimage *svgKnightLight;
extern struct NSVGimage *svgBishopLight;
extern struct NSVGimage *svgQueenLight;
extern struct NSVGimage *svgKingLight;

extern std::vector<tinyobj::shape_t> modelPawn;
extern std::vector<tinyobj::shape_t> modelRook;
extern std::vector<tinyobj::shape_t> modelKnight;
extern std::vector<tinyobj::shape_t> modelBishop;
extern std::vector<tinyobj::shape_t> modelQueen;
extern std::vector<tinyobj::shape_t> modelKing;

extern std::vector<tinyobj::shape_t> modelBoard;
extern std::vector<tinyobj::material_t> materialBoard;

bool Asset_Init(void);
void Asset_Quit(void);
