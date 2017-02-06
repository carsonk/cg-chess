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


bool Camera_Init(void)
{
    return true;
}


void Camera_Logic(uint32_t currentTick)
{

}


void Camera_Quit(void)
{

}
