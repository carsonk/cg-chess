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
#include "render.h"
#include "SDL.h"

#define FIRST_AVAILABLE_DEVICE -1

static bool isInitialized;
static RenderType currentRenderType;
static SDL_Renderer *sdlRenderer;


static void Init2DVector(void)
{
	SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);
}


static void Render2DVector(uint32_t currentTick, double interpolation)
{
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 0);
	SDL_RenderClear(sdlRenderer);

	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdlRenderer, 0, 0, 639, 479);
	SDL_RenderDrawLine(sdlRenderer, 639, 0, 0, 479);
	
	SDL_RenderPresent(sdlRenderer);
}


bool Render_Init(RenderType renderType, SDL_Window *sdlWindow, bool vSync)
{
	// Never initialize twice.
	assert(!isInitialized);

	// No renderer.
	if (renderType == RENDERTYPE_NONE)
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render_Init: Initialized successfully with RENDERTYPE_NONE.");
		isInitialized = true;
		currentRenderType = renderType;
		return true;
	}
	// Use SDL's renderer for basic 2D graphics.
	else if (renderType == RENDERTYPE_2D_VECTOR)
	{
		// Must associate SDL renderer with SDL window.
		assert(sdlWindow != NULL);

		uint32_t renderFlags = 0;
		renderFlags |= (vSync ? SDL_RENDERER_PRESENTVSYNC : 0);

		sdlRenderer = SDL_CreateRenderer(sdlWindow, FIRST_AVAILABLE_DEVICE, renderFlags);

		if (sdlRenderer == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render_Init: Failed to create SDL renderer.");
			return false;
		}

		// If vertical synchronization was requested, check that we got it.
		if (vSync)
		{
			SDL_RendererInfo sdlRendererInfo;
			if (SDL_GetRendererInfo(sdlRenderer, &sdlRendererInfo) != 0)
			{
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetRendererInfo: %s", SDL_GetError());
			}
			else
			{
				if ((sdlRendererInfo.flags & SDL_RENDERER_PRESENTVSYNC) != SDL_RENDERER_PRESENTVSYNC)
				{
					SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer: VSync was requested, but was unavailable.");
				}
				else
				{
					SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer: VSync is enabled.");
				}
			}
		}

		Init2DVector();

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Render_Init: Initialized successfully with RENDERTYPE_2D_VECTOR.");
		isInitialized = true;
		currentRenderType = renderType;
		return true;
	}
	else
	{
		assert(false);
		return false;
	}
}


void Render_Quit(void)
{
	// Must be initialized already.
	assert(isInitialized);

	if (currentRenderType == RENDERTYPE_NONE)
	{
		isInitialized = false;
	}
	else if (currentRenderType == RENDERTYPE_2D_VECTOR)
	{
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = NULL;
		isInitialized = false;
	}
}


void Render_Draw(uint32_t currentTick, double interpolation)
{
	assert(isInitialized);

	switch (currentRenderType)
	{
		case RENDERTYPE_NONE:
			break;
		case RENDERTYPE_2D_VECTOR:
			Render2DVector(currentTick, interpolation);
			break;
		default:
			break;
	}
}
