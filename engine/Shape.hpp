// Shape.hpp
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
#pragma once

#include "GraphicsContext.hpp"
#include "ogl/ElementBuffer.hpp"
#include "ogl/Vertex.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace engine
{
    // Abstract class for creating 2D shapes out of vertex and index buffer objects
    class Shape
    {
    public:
        virtual void Render(GraphicsContext& gc) = 0;
        virtual inline ogl::Vertex* GetVertices() { return vertices_; }
        virtual inline int GetNumVertices() { return numVertices_; }
        virtual void ModifyVertices() = 0;
    protected:
        ogl::Vertex* vertices_ = nullptr;
        unsigned int numVertices_ = 0;
        unsigned int* indices_ = nullptr;
        unsigned int numIndices_ = 0;
        ogl::ElementBuffer ebo_;
        ogl::VertexBuffer vbo_;
        ogl::VertexArray vao_;
    };
}
