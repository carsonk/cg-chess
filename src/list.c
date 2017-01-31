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

#include "list.h"


typedef struct Node
{
    void *item;
    struct Node *next;
    struct Node *previous;
} Node;

typedef struct List
{
    size_t count;
    Node *head;
    Node *tail;
} List;


// Internal function.
// Allocates a new node and initializes its item field.
static Node* CreateNode(void *newItem)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode != NULL)
        newNode->item = newItem;

    return newNode;
}


// Internal function.
// Gets the node at the specified index.
static Node* GetNodeAt(List *linkedList, size_t index)
{
    if (linkedList == NULL)
        return NULL;

    size_t count = linkedList->count;

    // Validate index.
    if (index >= linkedList->count)
        return NULL;

    Node *currentNode = NULL;
    // Search from head to tail.
    if (index <= ((count - 1) / 2))
    {
        currentNode = linkedList->head;
        for (size_t i = 0; i < index; i++)
            currentNode = currentNode->next;
    }
    // Search from tail to head.
    else
    {
        currentNode = linkedList->tail;
        for (size_t i = count - 1; i > index; i--)
            currentNode = currentNode->previous;
    }

    return currentNode;
}


void* List_Create(void)
{
    List *newList = malloc(sizeof(List));

    if (newList != NULL)
    {
        newList->count = 0;
        newList->head = NULL;
        newList->tail = NULL;
    }

    return newList;
}


void List_Destroy(void *list, void (*itemDestroyFunction)(void*))
{
    if (list == NULL)
        return;

    // Free all nodes and (potentially) destroy all items.
    List_Clear(list, itemDestroyFunction);

    // Free the list.
    free(list);
}


void List_Clear(void *list, void (*itemDestroyFunction)(void*))
{
    if (list == NULL)
        return;

    List *linkedList = (List*)list;

    // Start at the head of the list.
    Node *currentNode = linkedList->head;
    Node *nextNode = NULL;

    // While there are nodes to free.
    while (currentNode != NULL)
    {
        // Store the next node.
        nextNode = currentNode->next;

        // If a function to destroy items was provided, destroy the item.
        if (itemDestroyFunction != NULL)
            itemDestroyFunction(currentNode->item);

        // Free the current node.
        free(currentNode);

        // Set the next node as the current node.
        currentNode = nextNode;
    }

    // List was cleared.
    linkedList->count = 0;
    linkedList->head = NULL;
    linkedList->tail = NULL;
}


bool List_Add(void *list, size_t index, void *newItem)
{
    if (list == NULL)
        return false;

    List *linkedList = (List*)list;

    // Can specify insertion at the tail of the list.
    size_t maxIndex = linkedList->count;

    // Out of range.
    if (index > maxIndex)
        return false;

    // Insert at head.
    if (index == 0)
        return List_AddFirst(list, newItem);
    // Insert at tail.
    else if (index == maxIndex)
        return List_AddLast(list, newItem);
    // Insert at neither the head or tail.
    else
    {
        // Create a new node.
        Node *centerNode = CreateNode(newItem);
        if (centerNode == NULL)
            return false;

        // Get the node currently at the requested position.
        Node *rightNode = GetNodeAt(linkedList, index);
        Node *leftNode = rightNode->previous;

        // Setup center node pointers.
        centerNode->previous = leftNode;
        centerNode->next = rightNode;

        // Left node next points to center node.
        leftNode->next = centerNode;

        // Right node previous points to center node.
        rightNode->previous = centerNode;

        // Increment the count.
        linkedList->count++;

        return true;
    }
}


bool List_AddFirst(void *list, void *newItem)
{
    if (list == NULL)
        return false;

    // Create a new node.
    Node *newNode = CreateNode(newItem);
    if (newNode == NULL)
        return false;


    List *linkedList = (List*)list;


    // If the list is empty.
    if (linkedList->head == NULL)
    {
        newNode->next = NULL;
        newNode->previous = NULL;

        // New node is both head and tail.
        linkedList->head = newNode;
        linkedList->tail = newNode;
    }
    // If the list is not empty.
    else
    {
        Node *oldHead = linkedList->head;

        // New node next points to old head.
        newNode->next = oldHead;
        newNode->previous = NULL;

        // Old head previous points to new node.
        oldHead->previous = newNode;

        // New node is the new head.
        linkedList->head = newNode;
    }

    // Increment the count.
    linkedList->count++;

    return true;
}


bool List_AddLast(void *list, void *newItem)
{
    if (list == NULL)
        return false;

    // Create a new node.
    Node *newNode = CreateNode(newItem);
    if (newNode == NULL)
        return false;


    List *linkedList = (List*)list;


    // If the list is empty.
    if (linkedList->tail == NULL)
    {
        newNode->next = NULL;
        newNode->previous = NULL;

        // New node is both head and tail.
        linkedList->head = newNode;
        linkedList->tail = newNode;
    }
    // If the list is not empty.
    else
    {
        Node *oldTail = linkedList->tail;

        newNode->next = NULL;
        // New node previous points to old tail.
        newNode->previous = oldTail;

        // Old tail next points to new node.
        oldTail->next = newNode;

        // New node is the new tail.
        linkedList->tail = newNode;
    }

    // Increment the count.
    linkedList->count++;

    return true;
}


bool List_Remove(void *list, size_t index, void **removedItem)
{
     if (list == NULL)
        return false;
    else if (removedItem == NULL)
        return false;

    List *linkedList = (List*)list;

    size_t count = linkedList->count;

    // Validate index.
    if (index >= count)
        return false;

    // Remove head.
    if (index == 0)
        return List_RemoveFirst(list, removedItem);
    // Remove tail.
    else if (index == (count - 1))
        return List_RemoveLast(list, removedItem);
    // Remove any other node.
    else
    {
        Node *centerNode = GetNodeAt(linkedList, index);
        Node *leftNode = centerNode->previous;
        Node *rightNode = centerNode->next;

        *removedItem = centerNode->item;
        free(centerNode);

        leftNode->next = rightNode;
        rightNode->previous = leftNode;

        // Decrement the count.
        linkedList->count--;

        return true;
    }
}


bool List_RemoveFirst(void *list, void **removedItem)
{
    if (list == NULL)
        return false;
    if (removedItem == NULL)
        return false;

    List *linkedList = (List*)list;

    if (linkedList->head == NULL)
        return false;

    Node *nodeToDelete = linkedList->head;
    // New head might be NULL.
    Node *newHead = nodeToDelete->next;

    linkedList->head = newHead;

    *removedItem = nodeToDelete->item;
    free(nodeToDelete);

    // List is now empty.
    if (linkedList->head == NULL)
        linkedList->tail = NULL;
    // List is not empty. Adjust new head previous.
    else
        newHead->previous = NULL;

    // Decrement the count.
    linkedList->count--;

    return true;
}


bool List_RemoveLast(void *list, void **removedItem)
{
    if (list == NULL)
        return false;
    if (removedItem == NULL)
        return false;

    List *linkedList = (List*)list;

    if (linkedList->tail == NULL)
        return false;

    Node *nodeToDelete = linkedList->tail;
    // New tail might be NULL.
    Node *newTail = nodeToDelete->previous;

    linkedList->tail = newTail;

    *removedItem = nodeToDelete->item;
    free(nodeToDelete);

    // List is now empty.
    if (linkedList->tail == NULL)
        linkedList->head = NULL;
    // List is not empty. Adjust new head next.
    else
        newTail->next = NULL;

    // Decrement the count.
    linkedList->count--;

    return true;
}


bool List_Get(void *list, size_t index, void **existingItem)
{
    if (list == NULL)
        return false;
    else if (existingItem == NULL)
        return false;

    List *linkedList = (List*)list;

    if (index >= linkedList->count)
        return false;

    *existingItem = GetNodeAt(linkedList, index)->item;

    return true;
}


bool List_GetFirst(void *list, void **existingItem)
{
    if (list == NULL)
        return false;
    else if (existingItem == NULL)
        return false;

    List *linkedList = (List*)list;

    if (linkedList->head == NULL)
        return false;

    *existingItem = linkedList->head->item;

    return true;
}


bool List_GetLast(void *list, void **existingItem)
{
    if (list == NULL)
        return false;
    else if (existingItem == NULL)
        return false;

    List *linkedList = (List*)list;

    if (linkedList->tail == NULL)
        return false;

    *existingItem = linkedList->tail->item;

    return true;
}


size_t List_Count(void *list)
{
    if (list == NULL)
        return 0;
    else
        return ((List*)list)->count;
}
