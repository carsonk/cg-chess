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

typedef enum SVGAssetType
{
    SVG_PAWN_DARK,
    SVG_ROOK_DARK,
    SVG_KNIGHT_DARK,
    SVG_BISHOP_DARK,
    SVG_QUEEN_DARK,
    SVG_KING_DARK,
    SVG_PAWN_LIGHT,
    SVG_ROOK_LIGHT,
    SVG_KNIGHT_LIGHT,
    SVG_BISHOP_LIGHT,
    SVG_QUEEN_LIGHT,
    SVG_KING_LIGHT,
    SVG_ASSET_COUNT
} SVGAssetEnum;
extern struct NSVGimage **svgAssets;

extern tinyobj::attrib_t modelPawnAttrib;
extern std::vector<tinyobj::shape_t> modelPawnShape;

extern tinyobj::attrib_t modelRookAttrib;
extern std::vector<tinyobj::shape_t> modelRookShape;

extern tinyobj::attrib_t modelKnightAttrib;
extern std::vector<tinyobj::shape_t> modelKnightShape;

extern tinyobj::attrib_t modelBishopAttrib;
extern std::vector<tinyobj::shape_t> modelBishopShape;

extern tinyobj::attrib_t modelQueenAttrib;
extern std::vector<tinyobj::shape_t> modelQueenShape;

extern tinyobj::attrib_t modelKingAttrib;
extern std::vector<tinyobj::shape_t> modelKingShape;

extern tinyobj::attrib_t modelBoardAttrib;
extern std::vector<tinyobj::shape_t> modelBoardShape;
extern std::vector<tinyobj::material_t> modelBoardMaterial;

bool Asset_Init(void);
void Asset_Quit(void);
