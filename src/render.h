#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "SDL.h"

typedef enum RenderType
{
	RENDERTYPE_NONE,
	RENDERTYPE_2D_VECTOR
} RenderType;

bool Render_Init(RenderType renderType, SDL_Window *sdlWindow, bool vSync);
void Render_Quit(void);

void Render_Draw(uint32_t currentTick, double interpolation);
