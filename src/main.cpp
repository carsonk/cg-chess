#include <inttypes.h>
#include <stdbool.h>
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

	
	// Performance statistics.
	uint32_t currentFramesPerSecond = 0;
	uint32_t tickAtMeasurement = 0;


	// Game Loop
	// Fixed Logic Timestep, Variable Rendering
	// Robert Nystrom's Game Programming Patterns
	// http://gameprogrammingpatterns.com/game-loop.html
	uint32_t previousTime = SDL_GetTicks();
	uint32_t laggedTime = 0;
	uint32_t currentTick = 0;
	uint32_t currentFrame = 0;
	bool isRunning = true;
	while (isRunning)
	{
		// Understand that a "tick" means something different in the context of SDL, compared to the context of game logic.
		// An SDL "tick" is a millisecond.
		// A game "tick" is a logic step, with a time duration that we define.
		uint32_t currentTime = SDL_GetTicks();
		uint32_t elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		laggedTime += elapsedTime;

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

		while (laggedTime >= MS_PER_TICK)
		{
			// DoLogic

			currentTick++;
			laggedTime -= MS_PER_TICK;
		}

		// DoRender
		// (laggedTicks / MS_PER_UPDATE * 1.0)
		currentFrame++;


		// Performance statistics.
		if ((currentTick != 0) && (currentTick != tickAtMeasurement) && (currentTick % TICKS_PER_SECOND == 0))
		{
			currentFramesPerSecond = currentFrame - currentFramesPerSecond;
			tickAtMeasurement = currentTick;
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Tick: %" PRIu32 " Frame: %" PRIu32 " FPS: %" PRIu32, currentTick, currentFrame, currentFramesPerSecond);
		}
	}


	SDL_Quit();

	return EXIT_SUCCESS;
}
