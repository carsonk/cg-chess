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

    return true;
}


void Render_Quit(void)
{
}


void Render_Draw(uint32_t currentTick, double interpolation)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(sdlWindow);
}
