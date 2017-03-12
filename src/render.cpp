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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "asset.h"
#include "game.h"
#include "list.h"
#include "main.h"
#include "render.h"
#include "SDL.h"

#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"


static NSVGrasterizer *svgRasterizerContext;

typedef enum PieceTextureIndex
{
    DARK_PAWN_TEXTURE,
    DARK_ROOK_TEXTURE,
    DARK_KNIGHT_TEXTURE,
    DARK_BISHOP_TEXTURE,
    DARK_QUEEN_TEXTURE,
    DARK_KING_TEXTURE,
    LIGHT_PAWN_TEXTURE,
    LIGHT_ROOK_TEXTURE,
    LIGHT_KNIGHT_TEXTURE,
    LIGHT_BISHOP_TEXTURE,
    LIGHT_QUEEN_TEXTURE,
    LIGHT_KING_TEXTURE,
    PIECE_TEXTURE_COUNT
} PieceTextureIndex;
static SDL_Texture *pieceTextures[12];
static int svgDimension = 0;

static int viewportDimension;


void DestroySVGTextures(void)
{
    for (int i = 0; i < PIECE_TEXTURE_COUNT; i++)
        SDL_DestroyTexture(pieceTextures[i]);
}


void RasterizeSVGTextures(int size)
{
    // Allocate memory for rasterization.
    unsigned char *pixels = new unsigned char[size * size * 4];

    // Rasterize new textures.
    for (int i = 0; i < PIECE_TEXTURE_COUNT; i++)
    {
        // Last parameter is mislabeled by nanosvg.
        // Parameter is the width of a single row of data, not the stride.
        nsvgRasterize(svgRasterizerContext, svgAssets[i], 0, 0, 1, (unsigned char*)pixels, size, size, size * 4);

        // Allocate a new texture.
        if (pieceTextures[i] == NULL)
        {
            pieceTextures[i] = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, size, size);
            SDL_SetTextureBlendMode(pieceTextures[i], SDL_BLENDMODE_BLEND);
            SDL_UpdateTexture(pieceTextures[i], NULL, pixels, size * 4);
        }
        // Update the existing texture.
        else
        {
            SDL_DestroyTexture(pieceTextures[i]);
            pieceTextures[i] = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, size, size);
            SDL_SetTextureBlendMode(pieceTextures[i], SDL_BLENDMODE_BLEND);
            SDL_UpdateTexture(pieceTextures[i], NULL, pixels, size * 4);
        }
    }

    delete pixels;
}


bool Render_Init()
{
    SDL_Rect viewport;
    SDL_RenderGetViewport(sdlRenderer, &viewport);

    if (viewport.w < viewport.h)
        viewportDimension = viewport.w;
    else
        viewportDimension = viewport.h;

    svgRasterizerContext = nsvgCreateRasterizer();
    if (!svgRasterizerContext)
        return false;

    RasterizeSVGTextures(viewportDimension / 8.0f);

    return true;
}

static void ResizeViewport(int32_t width, int32_t height)
{
    int minimum = 0;
    bool heightIsMinimum = false;
    if (width > height)
    {
        minimum = height;
        heightIsMinimum = true;
    }
    else
    {
        minimum = width;
        heightIsMinimum = false;
    }

    viewportDimension = minimum;

    int letterBoxWidth = 0;
    if (heightIsMinimum)
    {
        letterBoxWidth = (width - height) / 2;

        SDL_Rect newViewport = { letterBoxWidth, 0, minimum, minimum };
        SDL_RenderSetViewport(sdlRenderer, &newViewport);
    }
    else
    {
        letterBoxWidth = (height - width) / 2;
        SDL_Rect newViewport = { 0, letterBoxWidth, minimum, minimum };
        SDL_RenderSetViewport(sdlRenderer, &newViewport);
    }
}


static void ProcessEvent(SDL_Event *sdlEvent)
{
    int drawableWidth = 0;
    int drawableHeight = 0;
    switch (sdlEvent->type)
    {
        case SDL_WINDOWEVENT:
            switch (sdlEvent->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_GL_GetDrawableSize(sdlWindow, &drawableWidth, &drawableHeight);
                    ResizeViewport(drawableWidth, drawableHeight);
                    RasterizeSVGTextures(viewportDimension / 8.0f);
                    break;
            }
            break;
    }
}


void Render_Logic(uint32_t currentTick)
{
    void *listIterator = List_IteratorCreate(sdlEventBuffer);

    SDL_Event *currentEvent;
    while (List_IteratorNext(listIterator, (void**)&currentEvent))
        ProcessEvent(currentEvent);
}


void Render_Quit(void)
{
    DestroySVGTextures();
    nsvgDeleteRasterizer(svgRasterizerContext);
}


void Render_Draw(uint32_t currentTick, double interpolation)
{
    SDL_SetRenderDrawColor(sdlRenderer, 64, 64, 64, 64);
    SDL_RenderClear(sdlRenderer);

    bool lightChecker = true;
    int y = 0;
    int xInc = viewportDimension / 8.0f;
    int yInc = viewportDimension / 8.0f;
    for (int rank = 0; rank < NUM_RANKS; rank++)
    {
        float x = 0;
        for (int file = 0; file < NUM_FILES; file++)
        {
            if (lightChecker)
                SDL_SetRenderDrawColor(sdlRenderer, 255, 206, 158, 255);
            else
                SDL_SetRenderDrawColor(sdlRenderer, 209, 139, 71, 255);
            SDL_Rect checker = { x, y, xInc, yInc };
            SDL_RenderFillRect(sdlRenderer, &checker);
            SDL_Rect piece = { x + (xInc / 8), y + (yInc / 8), xInc, yInc };

            switch (boardState.pieces[file][rank])
            {
            case PIECE_PAWN:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_PAWN_TEXTURE], NULL, &piece);
                break;
            case PIECE_ROOK:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_ROOK_TEXTURE], NULL, &piece);
                break;
            case PIECE_KNIGHT:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_KNIGHT_TEXTURE], NULL, &piece);
                break;
            case PIECE_BISHOP:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_BISHOP_TEXTURE], NULL, &piece);
                break;
            case PIECE_QUEEN:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_QUEEN_TEXTURE], NULL, &piece);
                break;
            case PIECE_KING:
                SDL_RenderCopy(sdlRenderer, pieceTextures[LIGHT_KING_TEXTURE], NULL, &piece);
                break;

            case PIECE_BPAWN:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_PAWN_TEXTURE], NULL, &piece);
                break;
            case PIECE_BROOK:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_ROOK_TEXTURE], NULL, &piece);
                break;
            case PIECE_BKNIGHT:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_KNIGHT_TEXTURE], NULL, &piece);
                break;
            case PIECE_BBISHOP:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_BISHOP_TEXTURE], NULL, &piece);
                break;
            case PIECE_BQUEEN:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_QUEEN_TEXTURE], NULL, &piece);
                break;
            case PIECE_BKING:
                SDL_RenderCopy(sdlRenderer, pieceTextures[DARK_KING_TEXTURE], NULL, &piece);
                break;
            }
            x += xInc;

            if (lightChecker)
                lightChecker = false;
            else
                lightChecker = true;
        }

        if (lightChecker)
            lightChecker = false;
        else
            lightChecker = true;

        y += yInc;
    }

    SDL_RenderPresent(sdlRenderer);
}
