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

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "asset.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "list.h"
#include "main.h"
#include "render.h"
#include "sample.h"
#include "SDL.h"


#define WINDOW_TITLE "cg-chess"
#define WINDOW_DEFAULT_WIDTH (512)
#define WINDOW_DEFAULT_HEIGHT (512)

#define TICKS_PER_SECOND (60)
#define MS_PER_TICK (1000 / TICKS_PER_SECOND)


bool isRunning = false;
void *sdlEventBuffer = NULL;
SDL_Window *sdlWindow = NULL;
SDL_GLContext sdlGLContext;


static void DoInput(uint32_t currentTick)
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                SDL_Event *bufferedEvent = (SDL_Event*)malloc(sizeof(SDL_Event));
                memcpy(bufferedEvent, &sdlEvent, sizeof(SDL_Event));
                List_AddLast(sdlEventBuffer, bufferedEvent);
                break;
        }
    }
}


static void DoLogic(uint32_t currentTick)
{
    // Demonstration logic.
    Sample_Logic(currentTick);

    Camera_Logic(currentTick);
    Input_Logic(currentTick);
    Game_Logic(currentTick);

    List_Clear(sdlEventBuffer, free);
}


static void DoRender(uint32_t currentTick, double interpolation)
{
    Render_Draw(currentTick, interpolation);
}


int main(int argc, char *argv[])
{
    int retCode = EXIT_FAILURE;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init", SDL_GetError(), NULL);
        return EXIT_FAILURE;
    }

    // Create an SDL window.
    sdlWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    // Check if window was created successfully.
    if (sdlWindow == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow", SDL_GetError(), NULL);
        goto cleanup;
    }

    // Create an OpenGL context.
    sdlGLContext = SDL_GL_CreateContext(sdlWindow);
    if (sdlGLContext == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_GL_CreateContext", SDL_GetError(), NULL);
        goto cleanup;
    }


    // Initialize subsystems.

    // Asset Subsystem
    if (!Asset_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Asset_Init", "Failed to initialize asset subsystem.", NULL);
        goto cleanup;
    }

    // Input Subsystem
    if (!Input_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Input_Init", "Failed to initialize input subsystem.", NULL);
        goto cleanup;
    }

    // Camera Subsystem
    if (!Camera_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Camera_Init", "Failed to initialize camera subsystem.", NULL);
        goto cleanup;
    }

    // Game Subsystem
    if (!Game_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game_Init", "Failed to initialize game subsystem.", NULL);
        goto cleanup;
    }

    // Render Subsystem
    if (!Render_Init(true))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Render_Init", "Failed to initialize render subsystem.", NULL);
        goto cleanup;
    }

    // Allocate SDL event buffer.
    sdlEventBuffer = List_Create();
    if (sdlEventBuffer == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "main", "Failed to create buffer for SDL events.", NULL);
        goto cleanup;
    }


    // Performance statistics.
    uint32_t currentFramesPerSecond = 0;
    uint32_t lastMeasurementTick = 0;
    uint32_t lastMeasurementFrame = 0;


    // Game Loop
    // Fixed Logic Timestep, Variable Rendering
    // Robert Nystrom's Game Programming Patterns
    // http://gameprogrammingpatterns.com/game-loop.html
    uint32_t previousTime = SDL_GetTicks();
    uint32_t laggedTime = 0;
    uint32_t currentTick = 0;
    uint32_t currentFrame = 0;
    isRunning = true;
    while (isRunning)
    {
        // Understand that a "tick" means something different in the context of SDL, compared to the context of game logic.
        // An SDL "tick" is a millisecond.
        // A game "tick" is a logic step, with a time duration that we define.
        uint32_t currentTime = SDL_GetTicks();
        uint32_t elapsedTime = currentTime - previousTime;

        // Handle overflow of SDL ticks.
        if (currentTime < previousTime)
        {
            elapsedTime = currentTime + (UINT32_MAX - previousTime) + 1;
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL ticks overflowed: currentTime: %" PRIu32 "ms previousTime: %" PRIu32 "ms", currentTime, previousTime);
        }

        previousTime = currentTime;
        laggedTime += elapsedTime;


        // Core Input Function
        DoInput(currentTick);


        while (laggedTime >= MS_PER_TICK)
        {
            // Core Logic Function
            DoLogic(currentTick);

            currentTick++;

            laggedTime -= MS_PER_TICK;
        }


        // Core Render Function
        DoRender(currentTick, (laggedTime / (MS_PER_TICK * 1.0)));
        currentFrame++;


        // Performance statistics.
        // At least one second before logging.
        if ((currentTick - lastMeasurementTick) >= TICKS_PER_SECOND)
        {
            currentFramesPerSecond = currentFrame - lastMeasurementFrame;
            lastMeasurementTick = currentTick;
            lastMeasurementFrame = currentFrame;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Tick: %" PRIu32 " Frame: %" PRIu32 " FPS: %" PRIu32, currentTick, currentFrame, currentFramesPerSecond);
        }
    }

    retCode = EXIT_SUCCESS;

cleanup:
    List_Destroy(sdlEventBuffer, free);

    // Quit subsystems.
    Render_Quit();
    Game_Quit();
    Camera_Quit();
    Input_Quit();
    Asset_Quit();

    SDL_GL_DeleteContext(sdlGLContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return retCode;
}
