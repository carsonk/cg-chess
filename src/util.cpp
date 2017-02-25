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

#include "glm/mat4x3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tiny_obj_loader.h"


std::vector<glm::mat4x3>* Util_FindBoundingBox(tinyobj::attrib_t attrib, std::vector<tinyobj::shape_t> shapes)
{
    float minX = 0;
    float maxX = 0;
    float minY = 0;
    float maxY = 0;
    float minZ = 0;
    float maxZ = 0;

    for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
    {
        size_t indexOffset = 0;
        for (size_t faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); faceIndex++)
        {
            int faceVertices = shapes[shapeIndex].mesh.num_face_vertices[faceIndex];
            for (size_t vertexIndex = 0; vertexIndex < faceVertices; vertexIndex++)
            {
                tinyobj::index_t vertex = shapes[shapeIndex].mesh.indices[indexOffset + vertexIndex];

                float x = attrib.vertices[3 * vertex.vertex_index + 0];
                float y = attrib.vertices[3 * vertex.vertex_index + 1];
                float z = attrib.vertices[3 * vertex.vertex_index + 2];

                if (x < minX)
                    minX = x;
                if (x > maxX)
                    maxX = x;

                if (y < minY)
                    minY = y;
                if (y > maxY)
                    maxY = y;

                if (z < minZ)
                    minZ = z;
                if (z > maxZ)
                    maxZ = z;
            }
            indexOffset += faceVertices;
        }
    }

    // This model better be centered already...

    std::vector<glm::mat4x3> *planes = new std::vector<glm::mat4x3>;

    // Front
    glm::mat4x3 frontPlane = glm::mat4x3(
        minX, maxY, maxZ,
        maxX, maxY, maxZ,
        maxX, minY, maxZ,
        minX, minY, maxZ
    );
    planes->push_back(frontPlane);

    // Back
    glm::mat4x3 backPlane = glm::mat4x3(
        maxX, maxY, minZ,
        minX, maxY, minZ,
        minX, minY, minZ,
        maxX, minY, minZ
    );
    planes->push_back(backPlane);

    // Left
    glm::mat4x3 leftPlane = glm::mat4x3(
        minX, maxY, minZ,
        minX, maxY, maxZ,
        minX, minY, maxZ,
        minX, minY, minZ
    );
    planes->push_back(leftPlane);

    // Right
    glm::mat4x3 rightPlane = glm::mat4x3(
        maxX, maxY, maxZ,
        maxX, maxY, minZ,
        maxX, minY, minZ,
        maxX, minY, maxZ
    );
    planes->push_back(rightPlane);

    // Top
    glm::mat4x3 topPlane = glm::mat4x3(
        minX, maxY, minZ,
        maxX, maxY, minZ,
        maxX, maxY, maxZ,
        minX, maxY, maxZ
    );
    planes->push_back(topPlane);

    // Bottom
    glm::mat4x3 bottomPlane = glm::mat4x3(
        minX, minY, maxZ,
        minX, minY, minZ,
        maxX, minY, minZ,
        maxX, minY, maxZ
    );
    planes->push_back(bottomPlane);

    return planes;
}
