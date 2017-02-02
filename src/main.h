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
#include "SDL.h"

// Globally accessible boolean that determines whether the game loop should continue.
extern bool isRunning;

// Globally accessible list that stores all SDL events for later processing in the game logic.
extern void *sdlEventBuffer;

// Globally accessible SDL window that represents the game window.
extern SDL_Window *sdlWindow;

// Globally accessible OpenGL context that represents the game OpenGL context.
extern SDL_GLContext sdlGLContext;
