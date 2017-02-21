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
        /*case SDL_MOUSEMOTION:
            printf("Sample_Logic: The mouse moved to (%d, %d).\n", sdlEvent->motion.x, sdlEvent->motion.y);
            printf("It moved by (%d, %d) from its last recorded position.\n", sdlEvent->motion.xrel, sdlEvent->motion.yrel);
            break;
        */
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
    // Iterate through the event buffer, processing events.

    // Create an iterator for the event buffer.
    void *listIterator = List_IteratorCreate(sdlEventBuffer);

    // Pointer to stored event.
    SDL_Event *currentEvent;
    // Get the next event, storing its location into the currentEvent pointer.
    // If List_IteratorNext() returns false, no more events are available.
    while (List_IteratorNext(listIterator, (void**)&currentEvent))
    {
        ProcessEvent(currentEvent); // Process the event.

        // For example, consume keydown events.
        // Remember that this is a global event buffer.
        // If events are consumed here, no input functions hereafter will see the consumed events.

        // Iterator makes it safe to remove the current item from the list.
        if (currentEvent->type == SDL_KEYDOWN)
        {
            // When an item is removed from the list, it must be freed manually.
            // The list has no knowledge of the allocation / freeing methods used for the items it contains.
            // malloc() was used to allocate these items, so free() should be used to free them.

            // If an item is removed but not freed, a memory leak will occur.

            void *removedItem;

            // Note that removedItem and currentEvent will point to the same data.
            // Once free(removedItem) is called, currentEvent will point to garbage data.
            // currentEvent cannot be dereferenced after this removal.

            // If List_IteratorRemove() returns false, it failed to remove the item from the list.
            if (!List_IteratorRemove(listIterator, &removedItem))
                break; // Serious error. Corruption of list or iterator has occurred.
            else
                free(removedItem); // Free the item, now that it has been removed from the list.

            printf("Sample_Logic: Consumed keydown event in global buffer.\n");
        }
    }

    // Destroy the event buffer iterator.
    List_IteratorDestroy(listIterator);
}
