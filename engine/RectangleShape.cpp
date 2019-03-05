// RectangleShape.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
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

#include "RectangleShape.hpp"

namespace engine
{
    RectangleShape::RectangleShape(float left, float top, float right, float bottom)
    {
        numVertices_ = 4;
        vertices_ = new ogl::Vertex[numVertices_];
        vertices_[0] = {{left,top,0.f},{255,255,255,255},{0.f,0.f}};
        vertices_[1] = {{left,bottom,0.f},{255,255,255,255},{0.f,1.f}};
        vertices_[2] = {{right,bottom,0.f},{255,255,255,255},{1.f,1.f}};
        vertices_[3] = {{right,top,0.f},{255,255,255,255},{1.f,0.f}};
        numIndices_ = 6;
        indices_ = new unsigned int[numIndices_];
        indices_[0] = 0; indices_[1] = 1; indices_[2] = 2;
        indices_[3] = 0; indices_[4] = 2; indices_[5] = 3;
        vbo_.SetData(sizeof(ogl::Vertex)*numVertices_, vertices_, GL_STATIC_DRAW);
        ebo_.SetData(sizeof(unsigned int)*numIndices_, indices_, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        // vao_.Bind();
        vbo_.Bind();
        ebo_.Bind();
        vao_.AddBuffer(vbo_, vbl);
    }

    RectangleShape::~RectangleShape()
    {
        delete [] vertices_;
        delete [] indices_;
    }

    void RectangleShape::ModifyVertices()
    {
        vbo_.SetData(sizeof(ogl::Vertex) * numVertices_, vertices_, GL_STATIC_DRAW);
    }

    void RectangleShape::Render(GraphicsContext& gc)
    {
        gc.UseProgram();
        vao_.Bind();
        glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, nullptr);
    }
}