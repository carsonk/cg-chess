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

#pragma once

#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C"
{
#endif


void* List_Create(void);
void List_Destroy(void *list, void (*itemDestroyFunction)(void*));

void List_Clear(void *list, void (*itemDestroyFunction)(void*));

bool List_Add(void *list, size_t index, void *newItem);
bool List_AddFirst(void *list, void *newItem);
bool List_AddLast(void *list, void *newItem);

bool List_Remove(void *list, size_t index, void **removedItem);
bool List_RemoveFirst(void *list, void **removedItem);
bool List_RemoveLast(void *list, void **removedItem);

bool List_Get(void *list, size_t index, void **existingItem);
bool List_GetFirst(void *list, void **existingItem);
bool List_GetLast(void *list, void **existingItem);

size_t List_Count(void *list);


#ifdef __cplusplus
}
#endif
