// CircleShape.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
//-----------------------------------------------------------------------------
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see < https://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

#include "CircleShape.h"

#include <glm/glm.hpp>

namespace engine
{
    CircleShape::CircleShape(float x, float y, float radius, int precision)
    {
        if(precision < 5)
            precision = 5;
        numVertices_ = precision + 2;
        vertices_ = new ogl::Vertex[numVertices_];
        vertices_[0] = {{x, y, 0.f}, {255,255,255,255}, {0.5f, 0.5f}};
        for(unsigned int i=1; i < numVertices_ - 1; ++i)
        {
            float theta = 2.f * 3.1415926f * (float)i / (float)precision;
            vertices_[i].position.x = glm::sin(theta) * radius + x;
            vertices_[i].position.y = glm::cos(theta) * radius + y;
            vertices_[i].position.z = 0.f;
            vertices_[i].color = {255,255,255,255};
            vertices_[i].texCoords.s = glm::sin(theta);
            vertices_[i].texCoords.t = glm::cos(theta);
        }
        vertices_[numVertices_-1] = vertices_[1];
        vbo_.SetData(sizeof(ogl::Vertex)*numVertices_, vertices_, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vbo_.Bind();
        vao_.AddBuffer(vbo_, vbl);
    }
        
    CircleShape::~CircleShape()
    {
        delete [] vertices_;
    }
        
    void CircleShape::Render(GraphicsContext& gc)
    {
        vao_.Bind();
        gc.UseProgram();
        glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices_);
    }
        
    void CircleShape::ModifyVertices()
    {
        vbo_.Bind();
        vbo_.SetData(sizeof(ogl::Vertex)*numVertices_, vertices_, GL_STATIC_DRAW);
    }
}