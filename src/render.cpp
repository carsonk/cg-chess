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

// Must include Windows header prior to including OpenGL header.
#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl/GL.h>
#include <gl/GLU.h>


bool Render_Init(bool vSync)
{
    if (vSync)
    {
        if (SDL_GL_SetSwapInterval(1) != 0)
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_SetSwapInterval: %s", SDL_GetError());
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    int letterBoxWidth = 0;
    if (heightIsMinimum)
    {
        letterBoxWidth = (width - height) / 2;
        glViewport(letterBoxWidth, 0, minimum, minimum);
    }
    else
    {
        letterBoxWidth = (height - width) / 2;
        glViewport(0, letterBoxWidth, minimum, minimum);
    }
}


static void ProcessEvent(SDL_Event *sdlEvent)
{
    switch (sdlEvent->type)
    {
        case SDL_WINDOWEVENT:
            switch (sdlEvent->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    ResizeViewport(sdlEvent->window.data1, sdlEvent->window.data2);
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
    {
        ProcessEvent(currentEvent);

        if (currentEvent->type == SDL_KEYDOWN)
        {
            void *removedItem;

            if (!List_IteratorRemove(listIterator, &removedItem))
                break;
            else
                free(removedItem);
        }
    }
}


void Render_Quit(void)
{
}


void Render_Draw(uint32_t currentTick, double interpolation)
{
    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    bool lightChecker = true;
    float y = 1.0f;
    for (int row = 0; row < 8; row++)
    {
        float x = -1.0f;
        for (int col = 0; col < 8; col++)
        {
            if (lightChecker)
                glColor3f(255 / 255.0f, 206 / 255.0f, 158 / 255.0f);
            else
                glColor3f(209 / 255.0f, 139 / 255.0f, 71 / 255.0f);

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.25f, y);
            glVertex2f(x + 0.25f, y - 0.25f);
            glVertex2f(x, y - 0.25f);
            glEnd();

            x += 0.25f;

            if (lightChecker)
                lightChecker = false;
            else
                lightChecker = true;
        }

        if (lightChecker)
            lightChecker = false;
        else
            lightChecker = true;

        y -= 0.25f;
    }

    glFlush();

    SDL_GL_SwapWindow(sdlWindow);
}
