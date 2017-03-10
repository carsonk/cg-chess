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

#include "camera.h"
#include <stdio.h>
#include "list.h"                       // List data structure.
#include "main.h"                       // Reference the SDL event buffer list provided by the game loop.
#include "SDL.h"                        // For SDL_Event structure definition.
#include <glm/glm.hpp>                  // include GLM for vectors/matrices
#include <glm/gtc/matrix_transform.hpp> // Include matrix transform: lookAt, perspective
#include <glm/gtc/type_ptr.hpp>         // include type_ptr to convert mat4 to float[16]

// Must include Windows header prior to including OpenGL header.
#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl/GL.h>
#include <gl/GLU.h>


float FOV = 90;
float width = 512;
float height = 512;
float aspect = width / height;
bool mousePressed = false;
bool viewMode2D = true;
glm::vec3 cameraPos;
glm::vec3 cameraTarget;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;


bool Camera_ViewToModelView()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    return true;
}


bool Camera_ReloadProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    return true;
}


void SetViewMode()
{
    if (viewMode2D)
    {
        view = glm::mat4();
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    }
    else
    {
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        projection = glm::perspective(FOV, aspect, 0.1f, 100.0f);
    }

    Camera_ViewToModelView();
    Camera_ReloadProjection();
}


bool Camera_Init(void)
{
    //camera matrix tutorial http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
    //more specific guide for input https://learnopengl.com/#!Getting-started/Camera
    //create view matrix
    cameraPos = glm::vec3(0.0, 1.0, 3.0);
    cameraTarget = glm::vec3(0.0, 0.0, 1.0);
    cameraUp = glm::vec3(0.0, 1.0, 0.0);

    SetViewMode();

    return true;
}


// React to an SDL_Event.
void ProcessMotion(SDL_Event *sdlEvent)
{
    switch (sdlEvent->type)
    {
    case SDL_KEYDOWN:
        if (sdlEvent->key.keysym.sym == SDLK_t)
        {
            viewMode2D = !viewMode2D;
            SetViewMode();
        }
        break;
    case SDL_MOUSEMOTION:
        if (mousePressed && !viewMode2D)
        {
            //Arcball rotation with mouse motion https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
            glm::vec4 cameraFocusVector = glm::vec4(cameraPos - cameraTarget, 0);                                  /* create vector to target */
            glm::mat4 yawRotate = glm::rotate(glm::mat4(), float(sdlEvent->motion.xrel * 3.14 / 180.0), cameraUp); /* create rotation matrix */
            cameraFocusVector = yawRotate * cameraFocusVector;                                                     /* do the rotation */
            cameraPos = cameraFocusVector + glm::vec4(cameraTarget, 0);                                            /* change camera position */
            SetViewMode();
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousePressed = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mousePressed = false;
        break;
    case SDL_MOUSEWHEEL:
        int scrollAmt = sdlEvent->wheel.y;
        if ((FOV >= 90.5 && scrollAmt > 0) || (FOV <= 88.5 && scrollAmt < 0))
        {
            //do nothing
        }
        else
        {
            FOV = FOV + scrollAmt * 0.01;
        }
        SetViewMode();
        break;
    }
}


void Camera_Logic(uint32_t currentTick)
{
    void *listIterator = List_IteratorCreate(sdlEventBuffer);

    SDL_Event *currentEvent;
    while (List_IteratorNext(listIterator, (void **)&currentEvent))
        ProcessMotion(currentEvent);

    List_IteratorDestroy(listIterator);
}


void Camera_Quit(void)
{
}
