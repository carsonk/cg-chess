#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "SDL.h"
#include "main.h"
#include "Stockfish\src\position.h"

#define WINDOW_TITLE "cg-chess"
#define WINDOW_DEFAULT_WIDTH (640)
#define WINDOW_DEFAULT_HEIGHT (480)

#define TICKS_PER_SECOND (60)
#define MS_PER_TICK (1000 / TICKS_PER_SECOND)


int main(int argc, char *argv[])
{
	// Stockfish initialization.
	Position::init();

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


	// Game Loop
	// Fixed Logic Timestep, Variable Rendering
	// Robert Nystrom's Game Programming Patterns
	// http://gameprogrammingpatterns.com/game-loop.html
	uint32_t previousTicks = SDL_GetTicks();
	uint32_t laggedTicks = 0;
	bool isRunning = true;
	while (isRunning)
	{
		uint32_t currentTicks = SDL_GetTicks();
		uint32_t elapsedTicks = currentTicks - previousTicks;
		previousTicks = currentTicks;
		laggedTicks += elapsedTicks;

		// DoInput
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
			}
		}

		while (laggedTicks >= MS_PER_TICK)
		{
			// DoLogic
			laggedTicks -= MS_PER_TICK;
		}

		// DoRender
		// (laggedTicks / MS_PER_UPDATE * 1.0)
	}


	SDL_Quit();

	return EXIT_SUCCESS;
}
