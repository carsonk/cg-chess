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

#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "asset.h"
#include "SDL.h"
#include "ZipFile.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"


#define ASSET_PATH_SVG_PAWN_DARK "svg/Chess_pdt45.svg"
#define ASSET_PATH_SVG_ROOK_DARK "svg/Chess_rdt45.svg"
#define ASSET_PATH_SVG_KNIGHT_DARK "svg/Chess_ndt45.svg"
#define ASSET_PATH_SVG_BISHOP_DARK "svg/Chess_bdt45.svg"
#define ASSET_PATH_SVG_QUEEN_DARK "svg/Chess_qdt45.svg"
#define ASSET_PATH_SVG_KING_DARK "svg/Chess_kdt45.svg"

#define ASSET_PATH_SVG_PAWN_LIGHT "svg/Chess_plt45.svg"
#define ASSET_PATH_SVG_ROOK_LIGHT "svg/Chess_rlt45.svg"
#define ASSET_PATH_SVG_KNIGHT_LIGHT "svg/Chess_nlt45.svg"
#define ASSET_PATH_SVG_BISHOP_LIGHT "svg/Chess_blt45.svg"
#define ASSET_PATH_SVG_QUEEN_LIGHT "svg/Chess_qlt45.svg"
#define ASSET_PATH_SVG_KING_LIGHT "svg/Chess_klt45.svg"


#define ASSET_PATH_OBJ_PAWN "obj/pawn.obj"
#define ASSET_PATH_OBJ_ROOK "obj/rook.obj"
#define ASSET_PATH_OBJ_KNIGHT "obj/knight.obj"
#define ASSET_PATH_OBJ_BISHOP "obj/bishop.obj"
#define ASSET_PATH_OBJ_QUEEN "obj/queen.obj"
#define ASSET_PATH_OBJ_KING "obj/king.obj"
#define ASSET_PATH_OBJ_BOARD "obj/board.obj"

#define ASSET_PATH_MTL_BOARD "mtl/board.mtl"


const char *svgAssetPaths[] =
{
    ASSET_PATH_SVG_PAWN_DARK,
    ASSET_PATH_SVG_ROOK_DARK,
    ASSET_PATH_SVG_KNIGHT_DARK,
    ASSET_PATH_SVG_BISHOP_DARK,
    ASSET_PATH_SVG_QUEEN_DARK,
    ASSET_PATH_SVG_KING_DARK,

    ASSET_PATH_SVG_PAWN_LIGHT,
    ASSET_PATH_SVG_ROOK_LIGHT,
    ASSET_PATH_SVG_KNIGHT_LIGHT,
    ASSET_PATH_SVG_BISHOP_LIGHT,
    ASSET_PATH_SVG_QUEEN_LIGHT,
    ASSET_PATH_SVG_KING_LIGHT
};
#define SVG_ASSET_PATH_COUNT 12

const char *objAssetPaths[] =
{
    ASSET_PATH_OBJ_PAWN,
    ASSET_PATH_OBJ_ROOK,
    ASSET_PATH_OBJ_KNIGHT,
    ASSET_PATH_OBJ_BISHOP,
    ASSET_PATH_OBJ_QUEEN,
    ASSET_PATH_OBJ_KING,
    ASSET_PATH_OBJ_BOARD
};
#define OBJ_ASSET_PATH_COUNT 7

const char *objMaterialPaths[] =
{
    ASSET_PATH_MTL_BOARD
};
#define OBJ_ASSET_MATERIAL_PATH_COUNT 1


struct NSVGimage **svgAssets;

std::vector<tinyobj::attrib_t> *objAttributes;
std::vector<std::vector<tinyobj::shape_t>> *objShapes;
std::vector<std::vector<tinyobj::material_t>>  *objMaterials;


void ReadLinesAndCopy(std::shared_ptr<ZipArchiveEntry> archiveEntry, char **destination)
{
    // http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring
    std::istream *decompressionStream = archiveEntry->GetDecompressionStream();
    std::string allLines(std::istreambuf_iterator<char>(*decompressionStream), {});

    // http://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
    char *copiedString = new char[allLines.length() + 1];
    strcpy_s(copiedString, allLines.length() + 1, allLines.c_str());

    *destination = copiedString;
}


bool LoadSVGAssets(std::shared_ptr<ZipArchive> archive)
{
    char *currentText = NULL;

    svgAssets = (NSVGimage**)malloc(SVG_ASSET_COUNT * sizeof(NSVGimage*));

    // Load SVGs
    for (int i = 0; i < SVG_ASSET_PATH_COUNT; i++)
    {
        ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(svgAssetPaths[i]);
        if (archiveEntry == nullptr)
            return false;
        ReadLinesAndCopy(archiveEntry, &currentText);
        svgAssets[i] = nsvgParse(currentText, "px", 96);
        delete(currentText);
    }

    return true;
}


bool LoadModelAssets(std::shared_ptr<ZipArchive> archive)
{
    std::string error;
    std::vector<tinyobj::material_t> unusedMaterials;
    std::map<std::string, int> unusedMaterialMap;

    objAttributes = new std::vector<tinyobj::attrib_t>(OBJ_ASSET_ATTRIB_COUNT);
    objShapes = new std::vector<std::vector<tinyobj::shape_t>>(OBJ_ASSET_SHAPE_COUNT);
    objMaterials = new std::vector<std::vector<tinyobj::material_t>>(OBJ_ASSET_MATERIAL_COUNT);

    // Load Shapes
    for (int i = 0; i < OBJ_ASSET_PATH_COUNT; i++)
    {
        ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(objAssetPaths[i]);
        if (archiveEntry == nullptr)
            return false;
        if (!tinyobj::LoadObj(&(objAttributes->at(i)), &(objShapes->at(i)), &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
        {
            if (error.length() > 0)
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
            return false;
        }
    }

    // Load Materials
    for (int i = 0; i < OBJ_ASSET_MATERIAL_COUNT; i++)
    {
        ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(objMaterialPaths[i]);
        if (archiveEntry == nullptr)
        {
            if (error.length() > 0)
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
            return false;
        }

        tinyobj::LoadMtl(&unusedMaterialMap, &(objMaterials->at(i)), archiveEntry->GetDecompressionStream(), &error);
    }

    return true;
}


bool Asset_Init(void)
{
    ZipArchive::Ptr archive = ZipFile::Open("assets");

    // Load SVG Assets
    if (!LoadSVGAssets(archive))
    {
        return false;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: Loaded SVG assets.");

    if (!LoadModelAssets(archive))
    {
        return false;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: Loaded model assets.");

    return true;
}


void Asset_Quit(void)
{
    nsvgDelete(svgAssets[SVG_PAWN_DARK]);
    nsvgDelete(svgAssets[SVG_ROOK_DARK]);
    nsvgDelete(svgAssets[SVG_KNIGHT_DARK]);
    nsvgDelete(svgAssets[SVG_BISHOP_DARK]);
    nsvgDelete(svgAssets[SVG_QUEEN_DARK]);
    nsvgDelete(svgAssets[SVG_KING_DARK]);

    nsvgDelete(svgAssets[SVG_PAWN_LIGHT]);
    nsvgDelete(svgAssets[SVG_ROOK_LIGHT]);
    nsvgDelete(svgAssets[SVG_KNIGHT_LIGHT]);
    nsvgDelete(svgAssets[SVG_BISHOP_LIGHT]);
    nsvgDelete(svgAssets[SVG_QUEEN_LIGHT]);
    nsvgDelete(svgAssets[SVG_KING_LIGHT]);

    free(svgAssets);
}
