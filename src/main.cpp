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

#define MAX_FRAMESKIP (TICKS_PER_SECOND / 4)


static void DoInput(uint32_t currentTick)
{

}


static void DoLogic(uint32_t currentTick)
{

}


static void DoRender(uint32_t currentTick, double interpolation)
{

}


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


		// Core Input Function
		DoInput(currentTick);


		// Little event loop for testing.
		// Should be moved to input function.
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


	SDL_Quit();

	return EXIT_SUCCESS;
}
