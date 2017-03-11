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
#include "list.h"
#include "main.h"
#include "render.h"
#include "SDL.h"


static int viewportDimension;

bool Render_Init()
{
    SDL_Rect viewport;
    SDL_RenderGetViewport(sdlRenderer, &viewport);

    if (viewport.w < viewport.h)
        viewportDimension = viewport.w;
    else
        viewportDimension = viewport.h;

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
}


void Render_Draw(uint32_t currentTick, double interpolation)
{
    SDL_SetRenderDrawColor(sdlRenderer, 64, 64, 64, 64);
    SDL_RenderClear(sdlRenderer);

    bool lightChecker = true;
    float y = 0;
    float xInc = viewportDimension / 8.0f;
    float yInc = viewportDimension / 8.0f;
    for (int row = 0; row < 8; row++)
    {
        float x = 0;
        for (int col = 0; col < 8; col++)
        {
            if (lightChecker)
                SDL_SetRenderDrawColor(sdlRenderer, 255, 206, 158, 255);
            else
                SDL_SetRenderDrawColor(sdlRenderer, 209, 139, 71, 255);
            SDL_Rect checker = { x, y, x + xInc, y + yInc };
            SDL_RenderFillRect(sdlRenderer, &checker);
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
