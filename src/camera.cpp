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

/*
    Camera Notes:

    The camera is responsible for transforming the world into view, and applying
    either a perspective or orthographic projection transformation.

    Thus, the camera owns the View matrix and the Projection matrix.

    It should manage these matrices using the GLM library.

    When requested, it should set up the ModelView and Projection matrices
    according to its separately stored View and Projection matrices.

    The idea is that the Rendering subsystem will request that the camera set
    up the ModelView and Projection matrices, and will then perform its own
    transformations upon the ModelView matrix to achieve its independent goals.

    The camera should also have input processing in its logic stage.

    User input should affect both the View and Projection matrices.

    Keyboard, mouse, and game controller are possible input sources to be
    considered.

    Camera movement should be as smooth as possible.

    This may require that the camera implement an additional function and
    storage for View matrix interpolation prior to rendering.

    e.x.
    Camera_Interpolate(uint32_t currentTick, double interpolation)

    The camera will have a position, a looking direction, camera position
    velocity, and looking direction velocity.

    These may be implemented using GLM, which has support for vectors.

    Implementing interpolation will require that the camera has a logical
    position, interpolated position, logical looking direction, and interpolated
    looking direction.

*/


#include "camera.h"
#include <stdio.h>
#include "list.h" // List data structure.
#include "main.h" // Reference the SDL event buffer list provided by the game loop.
#include "SDL.h" // For SDL_Event structure definition.
#include <glm/glm.hpp> // include GLM for vectors/matrices
#include <glm/gtc/matrix_transform.hpp> // Include matrix transform: lookAt, perspective
#include <glm/gtc/type_ptr.hpp> // include type_ptr to convert mat4 to float[16]

// Must include Windows header prior to including OpenGL header.
#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

float FOV = 90;
float width = 512;
float height = 512;
float aspect = width / height;
bool mousePressed = false;
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
    projection = glm::perspective(FOV, //field of view,  
                                  aspect, //aspect ratio, window width/height
                                  0.1f, //near plane
                                  100.0f); //far plane
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    
    return true;
}

bool Camera_Init(void)
{
    //camera matrix tutorial http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
    //more specific guide for input https://learnopengl.com/#!Getting-started/Camera
    //create view matrix
    cameraPos = glm::vec3(0.0, 1.0, 3.0); /* position of camera*/
    cameraTarget = glm::vec3(0.0, 0.0, 1.0); /* vector for camera target */
    cameraUp = glm::vec3(0.0, 1.0, 0.0); /* up vector */
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    Camera_ViewToModelView();

    //create projection matrix
    projection = glm::perspective(FOV, //field of view
                                  aspect, //aspect ratio, window width/height
                                  0.1f, //near plane
                                  100.0f); //far plane
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
    return true;
}

// React to an SDL_Event.
void ProcessMotion(SDL_Event *sdlEvent)
{
    // An SDL_Event is a structure that can contain a lot of different event types.
    // https://wiki.libsdl.org/SDL_Event

    // Switch on the type of event.
    // "type" is from the enumeration "SDL_EventType"
    // https://wiki.libsdl.org/SDL_EventType?highlight=%28%5CbCategoryEnum%5Cb%29%7C%28CategoryEvents%29
    switch (sdlEvent->type)
    {
        case SDL_MOUSEMOTION:
            if (mousePressed) {
                //Arcball rotation with mouse motion https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
                glm::vec4 cameraFocusVector = glm::vec4(cameraPos - cameraTarget, 0); /* create vector to target */
                glm::mat4 yawRotate = glm::rotate(glm::mat4(), float(sdlEvent->motion.xrel * 3.14 / 180.0), cameraUp); /* create rotation matrix */
                cameraFocusVector = yawRotate * cameraFocusVector; /* do the rotation */
                cameraPos = cameraFocusVector + glm::vec4(cameraTarget, 0); /* change camera position */
                view = glm::lookAt(cameraPos, cameraTarget, cameraUp); /* change view matrix */
            }
                
            break;
        case SDL_MOUSEBUTTONDOWN:
            mousePressed = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mousePressed = false;
            break;
    }
}

void Camera_Logic(uint32_t currentTick)
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
        ProcessMotion(currentEvent); // Process the event.

                                    // For example, consume keydown events.
                                    // Remember that this is a global event buffer.
                                    // If events are consumed here, no input functions hereafter will see the consumed events.

                                    // Iterator makes it safe to remove the current item from the list.
    }

    // Destroy the event buffer iterator.
    List_IteratorDestroy(listIterator);
}

void Camera_Quit(void)
{

}
