#include <stdbool.h>
#include <stdlib.h>
#include "SDL.h"
#include "main.h"
#include "Stockfish\src\position.h"

int main(int argc, char *argv[])
{

	Position::init();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init", SDL_GetError(), NULL);

		return EXIT_FAILURE;
	}

	// SDL test initialization.

	SDL_Window *sdlWindow = SDL_CreateWindow("cg-chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

	if (sdlWindow == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow", SDL_GetError(), NULL);

		SDL_Quit();

		return EXIT_FAILURE;
	}

	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}
