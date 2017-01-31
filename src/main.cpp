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
#include "camera.h"
#include "game.h"
#include "input.h"
#include "list.h"
#include "main.h"
#include "render.h"
#include "SDL.h"


#define WINDOW_TITLE "cg-chess"
#define WINDOW_DEFAULT_WIDTH (640)
#define WINDOW_DEFAULT_HEIGHT (480)

#define TICKS_PER_SECOND (60)
#define MS_PER_TICK (1000 / TICKS_PER_SECOND)

#define MAX_FRAMESKIP (TICKS_PER_SECOND / 4)


bool isRunning;
void *sdlEventBuffer;


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
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init", SDL_GetError(), NULL);

        return EXIT_FAILURE;
    }

    // Create an SDL window.
    SDL_Window *sdlWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 0);

    // Check if window was created successfully.
    if (sdlWindow == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow", SDL_GetError(), NULL);

        SDL_Quit();

        return EXIT_FAILURE;
    }


    // Initialize subsystems.

    // Input Subsystem
    if (!Input_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Input_Init", "Failed to initialize input subsystem.", NULL);

        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    // Camera Subsystem
    if (!Camera_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Camera_Init", "Failed to initialize camera subsystem.", NULL);

        Input_Quit();

        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    // Game Subsystem
    if (!Game_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game_Init", "Failed to initialize game subsystem.", NULL);

        Camera_Quit();
        Input_Quit();

        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    // Render Subsystem
    if (!Render_Init(RENDERTYPE_2D_VECTOR, sdlWindow, true))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Render_Init", "Failed to initialize render subsystem.", NULL);

        Game_Quit();
        Camera_Quit();
        Input_Quit();

        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    // Allocate SDL event buffer.
    sdlEventBuffer = List_Create();
    if (sdlEventBuffer == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "main", "Failed to create buffer for SDL events.", NULL);

        Render_Quit();
        Game_Quit();
        Camera_Quit();
        Input_Quit();

        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();

        return EXIT_FAILURE;
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
    uint32_t currentFrameskip = 0;
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


        currentFrameskip = 0;
        while ((laggedTime >= MS_PER_TICK) && currentFrameskip < MAX_FRAMESKIP)
        {
            // Core Logic Function
            DoLogic(currentTick);

            currentTick++;
            currentFrameskip++;

            if (currentFrameskip == MAX_FRAMESKIP)
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Maximum frameskip reached: %" PRIu32 " frames", currentFrameskip);
            }

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


    List_Destroy(sdlEventBuffer, free);

    // Quit subsystems.
    Render_Quit();
    Game_Quit();
    Camera_Quit();
    Input_Quit();

    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
