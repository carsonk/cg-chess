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

#include "asset.h"
#include "SDL.h"
#include "ZipFile.h"


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

const char *svgPawnDark;
const char *svgRookDark;
const char *svgKnightDark;
const char *svgBishopDark;
const char *svgQueenDark;
const char *svgKingDark;

const char *svgPawnLight;
const char *svgRookLight;
const char *svgKnightLight;
const char *svgBishopLight;
const char *svgQueenLight;
const char *svgKingLight;


void ReadLinesAndCopy(std::shared_ptr<ZipArchiveEntry> archiveEntry, const char *destination)
{
    // http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring
    std::istream *decompressionStream = archiveEntry->GetDecompressionStream();
    std::string allLines(std::istreambuf_iterator<char>(*decompressionStream), {});

    // http://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
    char *copiedString = new char[allLines.length() + 1];
    strcpy_s(copiedString, allLines.length() + 1, allLines.c_str());

    destination = copiedString;
}


// Asset_Quit() should free copied strings.
bool LoadSVGAssets(std::shared_ptr<ZipArchive> archive)
{
    // Dark Pieces
    ZipArchiveEntry::Ptr archiveEntry = archive->GetEntry(PIECE_SVG_PAWN_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgPawnDark);

    archiveEntry = archive->GetEntry(PIECE_SVG_ROOK_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgRookDark);

    archiveEntry = archive->GetEntry(PIECE_SVG_KNIGHT_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgKnightDark);

    archiveEntry = archive->GetEntry(PIECE_SVG_BISHOP_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgBishopDark);

    archiveEntry = archive->GetEntry(PIECE_SVG_QUEEN_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgQueenDark);

    archiveEntry = archive->GetEntry(PIECE_SVG_KING_DARK);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgKingDark);

    // Light Pieces
    archiveEntry = archive->GetEntry(PIECE_SVG_PAWN_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgPawnLight);

    archiveEntry = archive->GetEntry(PIECE_SVG_ROOK_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgRookLight);

    archiveEntry = archive->GetEntry(PIECE_SVG_KNIGHT_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgKnightLight);

    archiveEntry = archive->GetEntry(PIECE_SVG_BISHOP_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgBishopLight);

    archiveEntry = archive->GetEntry(PIECE_SVG_QUEEN_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgQueenLight);

    archiveEntry = archive->GetEntry(PIECE_SVG_KING_LIGHT);
    if (archiveEntry == nullptr)
        return false;
    ReadLinesAndCopy(archiveEntry, svgQueenLight);

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

    return true;
}


void Asset_Quit(void)
{
    delete(svgPawnDark);
    delete(svgRookDark);
    delete(svgKnightDark);
    delete(svgBishopDark);
    delete(svgQueenDark);
    delete(svgKingDark);

    delete(svgPawnLight);
    delete(svgRookLight);
    delete(svgKnightLight);
    delete(svgBishopLight);
    delete(svgQueenLight);
    delete(svgKingLight);
}
