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


#define PIECE_SVG_PAWN_DARK "svg/Chess_pdt45.svg"
#define PIECE_SVG_ROOK_DARK "svg/Chess_rdt45.svg"
#define PIECE_SVG_KNIGHT_DARK "svg/Chess_ndt45.svg"
#define PIECE_SVG_BISHOP_DARK "svg/Chess_bdt45.svg"
#define PIECE_SVG_QUEEN_DARK "svg/Chess_qdt45.svg"
#define PIECE_SVG_KING_DARK "svg/Chess_kdt45.svg"

#define PIECE_SVG_PAWN_LIGHT "svg/Chess_plt45.svg"
#define PIECE_SVG_ROOK_LIGHT "svg/Chess_rlt45.svg"
#define PIECE_SVG_KNIGHT_LIGHT "svg/Chess_nlt45.svg"
#define PIECE_SVG_BISHOP_LIGHT "svg/Chess_blt45.svg"
#define PIECE_SVG_QUEEN_LIGHT "svg/Chess_qlt45.svg"
#define PIECE_SVG_KING_LIGHT "svg/Chess_klt45.svg"


#define PIECE_OBJ_PAWN "obj/pawn.obj"
#define PIECE_OBJ_ROOK "obj/rook.obj"
#define PIECE_OBJ_KNIGHT "obj/knight.obj"
#define PIECE_OBJ_BISHOP "obj/bishop.obj"
#define PIECE_OBJ_QUEEN "obj/queen.obj"
#define PIECE_OBJ_KING "obj/king.obj"

#define PIECE_OBJ_BOARD "obj/board.obj"
#define PIECE_MTL_BOARD "mtl/board.mtl"

struct NSVGimage **svgAssets;

tinyobj::attrib_t modelPawnAttrib;
std::vector<tinyobj::shape_t> modelPawnShape;

tinyobj::attrib_t modelRookAttrib;
std::vector<tinyobj::shape_t> modelRookShape;

tinyobj::attrib_t modelKnightAttrib;
std::vector<tinyobj::shape_t> modelKnightShape;

tinyobj::attrib_t modelBishopAttrib;
std::vector<tinyobj::shape_t> modelBishopShape;

tinyobj::attrib_t modelQueenAttrib;
std::vector<tinyobj::shape_t> modelQueenShape;

tinyobj::attrib_t modelKingAttrib;
std::vector<tinyobj::shape_t> modelKingShape;

tinyobj::attrib_t modelBoardAttrib;
std::vector<tinyobj::shape_t> modelBoardShape;
std::vector<tinyobj::material_t> modelBoardMaterial;

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

    // Dark Pieces
    ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(PIECE_SVG_PAWN_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_PAWN_DARK] = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_ROOK_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_ROOK_DARK]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_KNIGHT_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_KNIGHT_DARK]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_BISHOP_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_BISHOP_DARK]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_QUEEN_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_QUEEN_DARK]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_KING_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_KING_DARK]  = nsvgParse(currentText, "px", 96);
    delete(currentText);


    // Light Pieces
    archiveEntry = archive->GetEntry(PIECE_SVG_PAWN_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_PAWN_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_ROOK_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_ROOK_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_KNIGHT_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_KNIGHT_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_BISHOP_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_BISHOP_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_QUEEN_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_QUEEN_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    archiveEntry = archive->GetEntry(PIECE_SVG_KING_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, &currentText);
    svgAssets[SVG_KING_LIGHT]  = nsvgParse(currentText, "px", 96);
    delete(currentText);

    return true;
}


bool LoadModelAssets(std::shared_ptr<ZipArchive> archive)
{
    std::string error;
    std::vector<tinyobj::material_t> unusedMaterials;
    std::map<std::string, int> unusedMaterialMap;

    ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(PIECE_OBJ_PAWN);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelPawnAttrib, &modelPawnShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_ROOK);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelRookAttrib, &modelRookShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_KNIGHT);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelKnightAttrib, &modelKnightShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_BISHOP);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelBishopAttrib, &modelBishopShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_QUEEN);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelQueenAttrib, &modelQueenShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_KING);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelKingAttrib, &modelKingShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_OBJ_BOARD);
    if (archiveEntry == nullptr)
        return false;
    if (!tinyobj::LoadObj(&modelBoardAttrib, &modelBoardShape, &unusedMaterials, &error, archiveEntry->GetDecompressionStream()))
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    archiveEntry = archive->GetEntry(PIECE_MTL_BOARD);
    if (archiveEntry == nullptr)
    {
        if (error.length() > 0)
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Asset_Init: LoadModelAssets: %s", error.c_str());
        return false;
    }

    tinyobj::LoadMtl(&unusedMaterialMap, &modelBoardMaterial, archiveEntry->GetDecompressionStream(), &error);


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
