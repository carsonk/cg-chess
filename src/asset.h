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

typedef enum SVGAssetIndex
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
} SVGAssetIndex;
extern struct NSVGimage **svgAssets;

typedef enum OBJAssetAttribIndex
{
    OBJ_PAWN_ATTRIB,
    OBJ_ROOK_ATTRIB,
    OBJ_KNIGHT_ATTRIB,
    OBJ_BISHOP_ATTRIB,
    OBJ_QUEEN_ATTRIB,
    OBJ_KING_ATTRIB,
    OBJ_BOARD_ATTRIB,
    OBJ_ASSET_ATTRIB_COUNT
} OBJAssetAttribIndex;
extern std::vector<tinyobj::attrib_t> *objAttributes;

typedef enum OBJAssetShapeIndex
{
    OBJ_PAWN_SHAPE,
    OBJ_ROOK_SHAPE,
    OBJ_KNIGHT_SHAPE,
    OBJ_BISHOP_SHAPE,
    OBJ_QUEEN_SHAPE,
    OBJ_KING_SHAPE,
    OBJ_BOARD_SHAPE,
    OBJ_ASSET_SHAPE_COUNT
} OBJAssetShapeIndex;
extern std::vector<std::vector<tinyobj::shape_t>> *objShapes;

typedef enum OBJAssetMaterialIndex
{
    OBJ_BOARD_MATERIAL,
    OBJ_ASSET_MATERIAL_COUNT
} OBJAssetMaterialIndex;
extern std::vector<std::vector<tinyobj::material_t>> *objMaterials;

bool Asset_Init(void);
void Asset_Quit(void);
