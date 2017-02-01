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

#include <stdio.h> // printf
#include "list.h" // List data structure.
#include "main.h" // Reference the SDL event buffer list provided by the game loop.
#include "SDL.h" // For SDL_Event structure definition.
#include "sample.h" // Define the functions in this header, in this source file.


// React to an SDL_Event.
void ProcessEvent(SDL_Event *sdlEvent)
{
    // An SDL_Event is a structure that can contain a lot of different event types.
    // https://wiki.libsdl.org/SDL_Event

    // Switch on the type of event.
    // "type" is from the enumeration "SDL_EventType"
    // https://wiki.libsdl.org/SDL_EventType?highlight=%28%5CbCategoryEnum%5Cb%29%7C%28CategoryEvents%29
    switch (sdlEvent->type)
    {
        const char *keyName;
        // A key was pressed.
        case SDL_KEYDOWN:
            // Access the SDL_KeyboardEvent structure for more information.
            // SDL_Event->SDL_KeyboardEvent.SDL_Keysym.SDL_Keycode

            // Let's get the friendly name of the key.
            keyName = SDL_GetKeyName(sdlEvent->key.keysym.sym);

            // We can even check if the key is held down, and print accordingly.

            // It is a "repeat" event. (i.e. The key is held and the OS is sending us another keydown event.)
            if (sdlEvent->key.repeat)
                printf("Sample_Logic: The %s key is held down.\n", keyName);
            else
                printf("Sample_Logic: The %s key was pressed.\n", keyName);
            break;
            // A key was released.
        case SDL_KEYUP:
            keyName = SDL_GetKeyName(sdlEvent->key.keysym.sym);
            printf("Sample_Logic: The %s key was released.\n", keyName);
            break;
            // A mouse button was pressed.
        case SDL_MOUSEBUTTONDOWN:
            // React to a left-click.
            if (sdlEvent->button.button == SDL_BUTTON_LEFT)
            {
                // React to the number of clicks.
                if (sdlEvent->button.clicks == 1)
                    printf("Sample_Logic: The left mouse button was clicked once at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
                else if (sdlEvent->button.clicks == 2)
                    printf("Sample_Logic: The left mouse button was clicked twice at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
                else
                    printf("Sample_Logic: The left mouse button was clicked many times at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
            }
            // React to a right-click.
            else if (sdlEvent->button.button == SDL_BUTTON_RIGHT)
            {
                // React to the number of clicks.
                if (sdlEvent->button.clicks == 1)
                    printf("Sample_Logic: The right mouse button was clicked once at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
                else if (sdlEvent->button.clicks == 2)
                    printf("Sample_Logic: The right mouse button was clicked twice at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
                else
                    printf("Sample_Logic: The right mouse button was clicked many times at (%d, %d).\n", sdlEvent->button.x, sdlEvent->button.y);
            }
            break;
            // A mouse button was released.
        case SDL_MOUSEBUTTONUP:
            if (sdlEvent->button.button == SDL_BUTTON_LEFT)
                printf("Sample_Logic: The left mouse button was released.\n");
            else if (sdlEvent->button.button == SDL_BUTTON_RIGHT)
                printf("Sample_Logic: The right mouse button was released.\n");
            break;
            // The mouse was moved.
        case SDL_MOUSEMOTION:
            printf("Sample_Logic: The mouse moved to (%d, %d).\n", sdlEvent->motion.x, sdlEvent->motion.y);
            printf("It moved by (%d, %d) from its last recorded position.\n", sdlEvent->motion.xrel, sdlEvent->motion.yrel);
            break;
    }
}


/*
    Example subsystem input-processing logic.
    A logic function may have more than input processing, but the input processing generally occurs first.

    "Logic Function"

    1. Process Input
    2. React to Input
    3. Other Functionality
*/
void Sample_Logic(uint32_t currentTick)
{
    // "sdlEventBuffer" is made available by including "main.h"
    // It is a list pointer owned by the game loop, so be careful with it.

    // Get the number of events stored in the list.
    size_t numberOfEventsInList = List_Count(sdlEventBuffer);

    // Iterate through the list, going from the first to the last event.
    // Do not consume (remove) events from the list when walking front-to-back.
    // If you need to consume events from the list, walk the list back-to-front.
    // Here, we are looking at all of the events but removing none of them.
    for (size_t currentEventIndex = 0; currentEventIndex < numberOfEventsInList; currentEventIndex++)
    {
        // Pointer to stored event.
        SDL_Event *currentEvent;

        // Retrieve the event at the specified index.
        if (List_Get(sdlEventBuffer, currentEventIndex, (void**)&currentEvent) == false)
        {
            // Failed to get an event from the specified index.
            // Something is seriously wrong here.
            // The list may be corrupted.
            break;
        }
        else
        {
            // Retrieved the event from the specified index successfully.
            // Let's analyze it.
            ProcessEvent(currentEvent);
        }
    }
}
